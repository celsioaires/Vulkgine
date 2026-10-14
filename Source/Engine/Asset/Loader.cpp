#include "Loader.h"

#include <fmt/core.h>
#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/parser.hpp>
#include <fastgltf/tools.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <stb/stb_image.h>

#include "../Render/Renderer.h"
#include "../Scene/MeshNode.h"

GltfAsset Loader::loadGltf(Renderer& renderer, std::filesystem::path path)
{
	GltfAsset gltf{};

	fastgltf::GltfDataBuffer data{};

	if (!data.loadFromFile(path))
	{
		fmt::println("Failed to open GLTF '{}'", path.string());
		return gltf;
	}

	fastgltf::GltfType type = fastgltf::determineGltfFileType(&data);

	fastgltf::Parser parser{};

	fastgltf::Options options =
		fastgltf::Options::DontRequireValidAssetMember |
		fastgltf::Options::AllowDouble |
		fastgltf::Options::LoadGLBBuffers |
		fastgltf::Options::LoadExternalBuffers;

	fastgltf::Asset asset{};

	switch (type)
	{
	case fastgltf::GltfType::glTF:
	{
		fastgltf::Expected<fastgltf::Asset> load = parser.loadGLTF(&data, path.parent_path(), options);

		if (load)
			asset = std::move(load.get());
		else
		{
			fmt::println("Failed to load GLTF: {}", fastgltf::to_underlying(load.error()));
			return gltf;
		}

		break;
	}
	case fastgltf::GltfType::GLB:
	{
		fastgltf::Expected<fastgltf::Asset> load = parser.loadBinaryGLTF(&data, path.parent_path(), options);

		if (load)
			asset = std::move(load.get());
		else
		{
			fmt::println("Failed to load binary GLTF: {}", fastgltf::to_underlying(load.error()));
			return gltf;
		}

		break;
	}
	}

	std::vector<TextureAsset> textures = loadTextures(renderer, asset);
	std::vector<MaterialAsset> materials = loadMaterials(renderer, asset, textures);
	std::vector<MeshAsset> meshes = loadMeshes(renderer, asset, materials);
	std::vector<Node*> nodes = loadNodes(asset, meshes);
	std::vector<Node*> topNodes = loadTopNodes(nodes);
	
	gltf.mTextures = textures;
	gltf.mMaterials = materials;
	gltf.mMeshes = meshes;
	gltf.mNodes = nodes;
	gltf.mTopNodes = topNodes;

	return gltf;
}

std::vector<TextureAsset> Loader::loadTextures(Renderer& renderer, fastgltf::Asset& asset)
{
	std::vector<TextureAsset> textures{};

	Pipeline& pipeline = renderer.getPipeline();

	// Texture
	TextureAsset textureAsset{};
	textureAsset.mGpuData.mSampler = pipeline.mNearestSampler;

	stbi_uc* pixels{};

	// Assets
	for (fastgltf::Image& image : asset.images)
	{
		int width{}, height{}, channels{};

		std::visit(
			fastgltf::visitor{
				[](auto& arg) {},
				[&](fastgltf::sources::URI& filePath)
				{
					assert(filePath.fileByteOffset == 0);
					assert(filePath.uri.isLocalPath());

					std::string path(filePath.uri.path().begin(), filePath.uri.path().end());

					pixels = stbi_load(path.c_str(), &width, &height, &channels, 4);
				},
				[&](fastgltf::sources::Vector& vector)
				{
					pixels = stbi_load_from_memory(vector.bytes.data(), (int)vector.bytes.size(), &width, &height, &channels, 4);
				},
				[&](fastgltf::sources::BufferView& view)
				{
					fastgltf::BufferView& bufferView = asset.bufferViews[view.bufferViewIndex];
					fastgltf::Buffer& buffer = asset.buffers[bufferView.bufferIndex];
					
					std::visit(
						fastgltf::visitor{
							[](auto& arg) {},
							[&](fastgltf::sources::Vector& vector)
							{
								pixels = stbi_load_from_memory(
									vector.bytes.data() + bufferView.byteOffset, 
									(int)bufferView.byteLength, 
									&width, 
									&height, 
									&channels, 
									4
								);
							}
						},
						buffer.data
					);
				},
			},
			image.data
		);

		if (pixels)
		{
			textureAsset.mGpuData.initializeImage(renderer, (void*)pixels, width, height);
			stbi_image_free(pixels);
		}
		else
		{
			// Checkerboard
			const int size = 16;
			std::array<uint32_t, size * size> checkerboard{};

			uint32_t color0 = glm::packUnorm4x8({ 1, 0, 0, 1 });
			uint32_t color1 = glm::packUnorm4x8({ 0, 0, 1, 1 });

			for (int y = 0; y < size; y++)
				for (int x = 0; x < size; x++)
					checkerboard[y * size + x] = ((x % 2) ^ (y % 2)) ? color0 : color1;

			textureAsset.mGpuData.initializeImage(renderer, checkerboard.data(), size, size);
		}

		textures.push_back(textureAsset);
	}

	return textures;
}

std::vector<MaterialAsset> Loader::loadMaterials(Renderer& renderer, fastgltf::Asset& asset, std::vector<TextureAsset>& textures)
{
	std::vector<MaterialAsset> materials{};

	Pipeline& pipeline = renderer.getPipeline();

	int i{};

	// Assets
	for (fastgltf::Material& material : asset.materials) 
	{
		// Constants
		MaterialConstants constants{};

		constants.mColorFactors.x = material.pbrData.baseColorFactor[0];
		constants.mColorFactors.y = material.pbrData.baseColorFactor[1];
		constants.mColorFactors.z = material.pbrData.baseColorFactor[2];
		constants.mColorFactors.w = material.pbrData.baseColorFactor[3];
		constants.mMetalRoughFactors.x = material.pbrData.metallicFactor;
		constants.mMetalRoughFactors.y = material.pbrData.roughnessFactor;

		// Color texture
		Texture colorTexture{};
		
		colorTexture.mSampler = pipeline.mLinearSampler;

		colorTexture.initializeWhiteImage(renderer);

		if (material.pbrData.baseColorTexture.has_value())
		{
			size_t imageIndex = asset.textures[material.pbrData.baseColorTexture.value().textureIndex]
				.imageIndex.value();

			colorTexture = textures[imageIndex].mGpuData;
		}

		// TODO: Metal rough texture

		// Material
		MaterialAsset materialAsset{};

		materialAsset.mConstants = constants;
		materialAsset.mGpuData.mColorTexture = colorTexture;

		materialAsset.mGpuData.initializeDescriptor(pipeline.mMaterialDescriptor, pipeline.mMaterialDescriptorLayout);
		materialAsset.mGpuData.initializeUbo(renderer);

		materialAsset.mGpuData.updateDescriptor(constants);

		materials.push_back(materialAsset);
		
		//colorTexture.cleanupInitialized();

		i++;
	}

	return materials;
}

std::vector<MeshAsset> Loader::loadMeshes(Renderer& renderer, fastgltf::Asset& asset, std::vector<MaterialAsset>& materials)
{
	std::vector<MeshAsset> meshes{};

	std::vector<Vertex> vertices{};
	std::vector<uint32_t> indices{};

	int i{};

	for (fastgltf::Mesh& mesh : asset.meshes)
	{
		vertices.clear();
		indices.clear();

		MeshAsset meshAsset{};

		if (!mesh.name.empty())
			meshAsset.mName = mesh.name;

		for (fastgltf::Primitive& primitive : mesh.primitives)
		{
			Geometry geometry{};

			geometry.mStartIndex = (uint32_t)indices.size();
			geometry.mIndexCount = (uint32_t)asset.accessors[primitive.indicesAccessor.value()].count;

			size_t initialVertex = vertices.size();

			// Indices
			fastgltf::Accessor& indexAccessor = asset.accessors[primitive.indicesAccessor.value()];
			
			indices.reserve(indices.size() + indexAccessor.count);

			fastgltf::iterateAccessor<std::uint32_t>(
				asset,
				indexAccessor,
				[&](std::uint32_t index) 
				{ 
					indices.push_back(index + (uint32_t)initialVertex); 
				}
			);

			// Vertex position
			fastgltf::Accessor& positionAccessor = asset.accessors[primitive.findAttribute("POSITION")->second];
			
			vertices.resize(vertices.size() + positionAccessor.count);

			fastgltf::iterateAccessorWithIndex<glm::vec3>(
				asset,
				positionAccessor,
				[&](glm::vec3 position, size_t index)
				{
					Vertex vertex{};

					vertex.mColor = glm::vec4(1);
					vertex.mNormal = { 1, 0, 0 };
					vertex.mPosition = position;

					vertices[initialVertex + index] = vertex;
				}
			);

			// Vertex uv
			fastgltf::Primitive::attribute_type* uvAttrib = primitive.findAttribute("TEXCOORD_0");

			if (uvAttrib != primitive.attributes.end())
				fastgltf::iterateAccessorWithIndex<glm::vec2>(
					asset,
					asset.accessors[(*uvAttrib).second],
					[&](glm::vec2 uv, size_t index)
					{ 
						vertices[initialVertex + index].mUv = uv; 
					}
				);

			// Vertex color
			fastgltf::Primitive::attribute_type* colorAttrib = primitive.findAttribute("COLOR_0");

			if (colorAttrib != primitive.attributes.end())
				fastgltf::iterateAccessorWithIndex<glm::vec4>(
					asset,
					asset.accessors[(*colorAttrib).second],
					[&](glm::vec4 color, size_t index)
					{
						vertices[initialVertex + index].mColor = color;
					}
				);

			// Vertex normal
			fastgltf::Primitive::attribute_type* normalAttrib = primitive.findAttribute("NORMAL");

			if (normalAttrib != primitive.attributes.end())
				fastgltf::iterateAccessorWithIndex<glm::vec3>(
					asset,
					asset.accessors[(*normalAttrib).second],
					[&](glm::vec3 normal, size_t index)
					{
						vertices[initialVertex + index].mNormal = normal;
					}
				);

			// Material
			if (primitive.materialIndex.has_value())
				geometry.mMaterial = materials[primitive.materialIndex.value()].mGpuData;
			else
				geometry.mMaterial = materials[0].mGpuData;

			meshAsset.mGeometries.push_back(geometry);
		}

		meshAsset.mGpuData.initializeBuffers(renderer, vertices, indices);

		meshes.emplace_back(std::move(meshAsset));

		i++;
	}

	return meshes;
}

std::vector<Node*> Loader::loadNodes(fastgltf::Asset& asset, std::vector<MeshAsset>& meshes)
{
	std::vector<Node*> nodes{};

	for (fastgltf::Node& node : asset.nodes)
	{
		Node* defaultNode{};

		// Mesh
		if (node.meshIndex.has_value())
		{
			auto* meshNode = new MeshNode;

			meshNode->mAsset = meshes[*node.meshIndex];

			defaultNode = meshNode;
		}
		else
			defaultNode = new Node;

		// Name
		if (!node.name.empty())
			defaultNode->mName = node.name;

		// Local transform
		std::visit(
			fastgltf::visitor{
				[&](fastgltf::Node::TransformMatrix transform)
				{
					memcpy(&defaultNode->mLocalTransform, transform.data(), sizeof(transform));
				},
				[&](fastgltf::Node::TRS transform)
				{
					glm::vec3 translation(transform.translation[0], transform.translation[1], transform.translation[2]);
					glm::quat rotation(transform.rotation[3], transform.rotation[0], transform.rotation[1], transform.rotation[2]);
					glm::vec3 scale(transform.scale[0], transform.scale[1], transform.scale[2]);

					defaultNode->mLocalTransform =
						glm::translate(glm::mat4(1), translation) *
						glm::toMat4(rotation) *
						glm::scale(glm::mat4(1), scale);
				}
			},
			node.transform
		);

		nodes.push_back(defaultNode);
	}

	int i{};

	// Transform hierarchy
	for (fastgltf::Node& node : asset.nodes)
	{
		Node* sceneNode = nodes[i];

		for (size_t children : node.children)
		{
			Node* n = nodes[children];

			sceneNode->mChildrens.push_back(n);
			n->mParent = sceneNode;
		}

		i++;
	}

	return nodes;
}

std::vector<Node*> Loader::loadTopNodes(std::vector<Node*>& nodes)
{
	std::vector<Node*> topNodes{};

	for (Node* node : nodes)
		if (!node->mParent)
		{
			topNodes.push_back(node);
			node->updateTransform(glm::mat4{ 1.f });
		}

	return topNodes;
}
