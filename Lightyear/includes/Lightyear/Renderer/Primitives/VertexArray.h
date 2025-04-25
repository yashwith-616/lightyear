#pragma once

#include "Lightyear/Renderer/Primitives/RenderTypes.h"
#include "Lightyear/LightyearCore.h"

namespace ly::renderer {

	class VertexBuffer;
	class IndexBuffer;

class VertexArray {
public:
	virtual ~VertexArray() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void AddVertexBuffer(const VertexBuffer& vertexBuffer) = 0;
	virtual void SetIndexBuffer(const IndexBuffer& indexBuffer) = 0;

	virtual const std::vector<VertexBuffer>& GetVertexBuffers() const = 0;
	virtual const IndexBuffer& GetIndexBuffer() const = 0;

	static VertexArray* Create();
};

}  // namespace ly::renderer