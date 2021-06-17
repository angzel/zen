#pragma once
#include <string>
#include "zen_metal_type.h"

namespace Zen { namespace Metal {

	struct BufferID;

	class Buffer
	{
	public:
		Buffer();

		void create(size_t len, void const * data = nullptr);

		void update(size_t offset, size_t size, void const * data);

		std::shared_ptr<BufferID> getID() const;
		
	protected:
		std::shared_ptr<BufferID> m_id = nullptr;
	};

	struct PipeID;

	class Pipe
	{
	public:
		Pipe(std::string const & name = "pipe");

		std::shared_ptr<PipeID> getID() const;

		std::string getName();

	protected:
		std::string m_name;
		std::shared_ptr<PipeID> m_id = nullptr;
	};
}}
