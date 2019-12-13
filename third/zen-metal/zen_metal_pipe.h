	// objective-c support needed

#if defined(ZEN_METAL_CPP)
#include <string>

namespace Zen { namespace Metal {

	struct BufferID;

	class Buffer
	{
	public:
		Buffer();

		void create(size_t len, void const * data = nullptr);

		void update(size_t offset, size_t size, void const * data);

		virtual ~Buffer();
		
		BufferID * getID() const;
	protected:
		BufferID * m_id = nullptr;
	};

	struct PipeID;

	class Pipe
	{
	public:
		Pipe(std::string const & name = "pipe");
		virtual ~Pipe();

		PipeID * getID() const;

		std::string getName();

	protected:
		std::string m_name;
		PipeID * m_id = nullptr;
	};
}}

#endif

#if defined(ZEN_METAL_OC)

#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>


namespace Zen { namespace Metal {
	struct BufferID
	{
		id <MTLBuffer> data;
	};
	
	struct PipeID
	{
		id <MTLRenderPipelineState> state;

		void create(id <MTLRenderPipelineState> & _state)
		{
			state = _state;
		}

		void create
		(std::string const & vertex_func,
		 std::string const & fragment_func);

		void create
		(std::string const & vertex_func,
		 std::string const & fragment_func,
		 eBlendFactor src_factor,
		 eBlendFactor dst_factor);

		void create
		(NSString * vertex_func_name,
		 NSString * fragment_func_name);

		void create
		(NSString * vertex_func_name,
		 NSString * fragment_func_name,
		 MTLBlendFactor src_factor,
		 MTLBlendFactor dst_factor);

	};
}}

#endif
