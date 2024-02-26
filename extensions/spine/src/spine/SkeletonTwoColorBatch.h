/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated September 24, 2021. Replaces all prior versions.
 *
 * Copyright (c) 2013-2021, Esoteric Software LLC
 *
 * Integration of the Spine Runtimes into software or otherwise creating
 * derivative works of the Spine Runtimes is permitted under the terms and
 * conditions of Section 2 of the Spine Editor License Agreement:
 * http://esotericsoftware.com/spine-editor-license
 *
 * Otherwise, it is permitted to integrate the Spine Runtimes into software
 * or otherwise create derivative works of the Spine Runtimes (collectively,
 * "Products"), provided that each user of the Products must obtain their own
 * Spine Editor license and redistribution of the Products in any form must
 * include this license and copyright notice.
 *
 * THE SPINE RUNTIMES ARE PROVIDED BY ESOTERIC SOFTWARE LLC "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ESOTERIC SOFTWARE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES,
 * BUSINESS INTERRUPTION, OR LOSS OF USE, DATA, OR PROFITS) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THE SPINE RUNTIMES, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#ifndef SPINE_SKELETONTWOCOLORBATCH_H_
#define SPINE_SKELETONTWOCOLORBATCH_H_

#include "axmol.h"
#include "renderer/backend/ProgramState.h"
#include <spine/spine.h>
#include <vector>

namespace spine {
	struct V3F_C4B_C4B_T2F {
		axmol::Vec3 position;
		axmol::Color4B color;
		axmol::Color4B color2;
		axmol::Tex2F texCoords;
	};

	struct TwoColorTriangles {
		V3F_C4B_C4B_T2F *verts;
		unsigned short *indices;
		int vertCount;
		int indexCount;
	};

	class SP_API TwoColorTrianglesCommand : public axmol::CustomCommand {
	public:
		TwoColorTrianglesCommand();

		~TwoColorTrianglesCommand();

		void init(float globalOrder, axmol::Texture2D *texture, axmol::backend::ProgramState *programState, axmol::BlendFunc blendType, const TwoColorTriangles &triangles, const axmol::Mat4 &mv, uint32_t flags);

		void updateCommandPipelineDescriptor(axmol::backend::ProgramState *programState);

		inline axmol::backend::TextureBackend *getTexture() const { return _texture; }

		void draw(axmol::Renderer *renderer);

		void updateVertexAndIndexBuffer(axmol::Renderer *renderer, V3F_C4B_C4B_T2F *vertices, int verticesSize, uint16_t *indices, int indicesSize);

		inline uint32_t getMaterialID() const { return _materialID; }

		inline const TwoColorTriangles &getTriangles() const { return _triangles; }

		inline ssize_t getVertexCount() const { return _triangles.vertCount; }

		inline ssize_t getIndexCount() const { return _triangles.indexCount; }

		inline const V3F_C4B_C4B_T2F *getVertices() const { return _triangles.verts; }

		inline const unsigned short *getIndices() const { return _triangles.indices; }

		inline axmol::BlendFunc getBlendType() const { return _blendType; }

		inline const axmol::Mat4 &getModelView() const { return _mv; }

		void setForceFlush(bool forceFlush) { _forceFlush = forceFlush; }

		bool isForceFlush() { return _forceFlush; };

	protected:
		void generateMaterialID();
		uint32_t _materialID;


		void *_prog = nullptr;
		axmol::backend::TextureBackend *_texture = nullptr;
		axmol::backend::ProgramState *_programState = nullptr;
		axmol::backend::UniformLocation _locPMatrix;
		axmol::backend::UniformLocation _locTexture;

		axmol::BlendFunc _blendType;
		TwoColorTriangles _triangles;
		axmol::Mat4 _mv;
		bool _forceFlush;
	};

	class SP_API SkeletonTwoColorBatch {
	public:
		static SkeletonTwoColorBatch *getInstance();

		static void destroyInstance();

		void update(float delta);

		V3F_C4B_C4B_T2F *allocateVertices(uint32_t numVertices);
		void deallocateVertices(uint32_t numVertices);

		unsigned short *allocateIndices(uint32_t numIndices);
		void deallocateIndices(uint32_t numIndices);

		TwoColorTrianglesCommand *addCommand(axmol::Renderer *renderer, float globalOrder, axmol::Texture2D *texture, axmol::backend::ProgramState *programState, axmol::BlendFunc blendType, const TwoColorTriangles &triangles, const axmol::Mat4 &mv, uint32_t flags);

		void batch(axmol::Renderer *renderer, TwoColorTrianglesCommand *command);

		void flush(axmol::Renderer *renderer, TwoColorTrianglesCommand *materialCommand);

		uint32_t getNumBatches() { return _numBatches; };

	protected:
		SkeletonTwoColorBatch();
		virtual ~SkeletonTwoColorBatch();

		void reset();

		TwoColorTrianglesCommand *nextFreeCommand();

		// pool of commands
		std::vector<TwoColorTrianglesCommand *> _commandsPool;
		uint32_t _nextFreeCommand;

		// pool of vertices
		std::vector<V3F_C4B_C4B_T2F> _vertices;
		uint32_t _numVertices;

		// pool of indices
		Vector<unsigned short> _indices;


		// VBO handles & attribute locations
		V3F_C4B_C4B_T2F *_vertexBuffer;
		uint32_t _numVerticesBuffer;
		uint32_t _numIndicesBuffer;
		unsigned short *_indexBuffer;

		// last batched command, needed for flushing to set material
		TwoColorTrianglesCommand *_lastCommand = nullptr;

		// number of batches in the last frame
		uint32_t _numBatches;
	};
}// namespace spine

#endif// SPINE_SKELETONTWOCOLORBATCH_H_
