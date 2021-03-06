#pragma once

#include <string>
#include "zen_color.h"

namespace Zen { namespace Metal {
	struct RenderID;

	class Render
	{
	protected:
		Render();
		virtual ~Render();
	public:
		static Render * Get();

		void setClearColor(Zen::Color4f);

		void start();

		void end();

		void activePipe(std::shared_ptr<PipeID> pipe_id);

		void setVertexBuffer(int index, std::shared_ptr<BufferID> buffer, size_t off);

		void setFragmentBuffer(int index, std::shared_ptr<BufferID> buffer, size_t off);
		
		void setFragmentBytes(int index, void const * bytes, size_t len);

		void bindTexture(int index, std::shared_ptr<TextureID> texture);

		void drawPrimitives(eVertexMode mode, size_t start, size_t count);

//		void drawIndexedPrimitives(eVertexMode mode, size_t indexCount, std::shared_ptr<BufferID> buffer, size_t off_bytes, size_t instanceCount);

		RenderID * getID() const;
	public:
		RenderID * m_id = nullptr;
		Zen::Color4f m_clear_color = Zen::eColor::Black;
	};
}}
