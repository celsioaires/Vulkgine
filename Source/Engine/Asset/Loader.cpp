#include "Loader.h"

#include <fmt/core.h>
#include <fastgltf/glm_element_traits.hpp>
#include <fastgltf/parser.hpp>
#include <fastgltf/tools.hpp>

std::vector<MeshAsset> Loader::loadMeshes(Renderer& renderer, std::filesystem::path path)
{
	std::vector<MeshAsset> meshes{};

	fastgltf::GltfDataBuffer data;

	if (!data.loadFromFile(path))
	{
		fmt::println("Failed to open GLTF: {}", path.string());
		return meshes;
	}

	fastgltf::Options options =
		fastgltf::Options::LoadGLBBuffers |
		fastgltf::Options::LoadExternalBuffers;

	fastgltf::Parser parser{};
	fastgltf::Expected<fastgltf::Asset> load = parser.loadBinaryGLTF(&data, path.parent_path(), options);

	fastgltf::Asset asset;

	if (load)
		asset = std::move(load.get());
	else
	{
		fmt::println("Failed to load GLTF: {}", fastgltf::to_underlying(load.error()));
		return meshes;
	}

	std::vector<Vertex> vertices{};
	std::vector<uint32_t> indices{};

	int i = 0;

	for (fastgltf::Mesh& mesh : asset.meshes)
	{
		MeshAsset meshAsset{};

		for (fastgltf::Primitive& primitive : mesh.primitives)
		{
			Geometry geometry{};
			geometry.mStartIndex = (uint32_t)indices.size();
			geometry.mIndexCount = (uint32_t)asset.accessors[primitive.indicesAccessor.value()].count;

			size_t initialVertex = vertices.size();

			// Indices
			fastgltf::Accessor& indexAccessor = asset.accessors[primitive.indicesAccessor.value()];
			indices.reserve(indices.size() + indexAccessor.count);

			fastgltf::iterateAccessor<std::uint32_t>(asset, indexAccessor,
				[&](std::uint32_t index) { indices.push_back(index + (uint32_t)initialVertex); }
			);

			// Vertex positions
			fastgltf::Accessor& positionAccessor = asset.accessors[primitive.findAttribute("POSITION")->second];
			vertices.resize(vertices.size() + positionAccessor.count);

			fastgltf::iterateAccessorWithIndex<glm::vec3>(asset, positionAccessor,
				[&](glm::vec3 position, size_t index)
				{
					Vertex vertex{};
					vertex.mPosition = position;
	
					vertices[initialVertex + index] = vertex;
				}
			);

			meshAsset.mGeometries.push_back(geometry);
		}

		meshAsset.mMesh.initializeBuffers(renderer, vertices, indices);

		meshes.emplace_back(std::move(meshAsset));

		i++;
	}	

	return meshes;
}
