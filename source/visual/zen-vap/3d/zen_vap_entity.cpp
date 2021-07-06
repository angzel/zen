
#include "zen_vap_entity.h"


namespace Zen { namespace Vap {
	Entity::Entity(std::string const & name)
	: Node2d(name)
	{
		
	}
	
	Color4f RectColorCalc(Color4f c0, Color4f c1, Color4f c2)
	{
		auto m = Color4fLerp(c1, c2, 0.5f);
		return Color4fLerp(c0, m, 2);
	}
	uint32_t RandomColor()
	{
		auto v = (uint32_t)rand();
		return (v<<8) | 0xff;
	}
	void Entity::setMesh(std::shared_ptr<Mesh> mesh)
	{
		m_mesh = mesh;
		m_colors.clear();
		Color c[24] = {
			RandomColor(), RandomColor(), RandomColor(), 0,
		};
		c[3] = RectColorCalc(c[0], c[1], c[2]);
		for(int i = 0; i < 24; i += 4)
		{
			c[i] = RandomColor();
			c[i+1] = c[i+2] = c[i+3] = c[i];
//			c[i+1] = RandomColor();
//			c[i+2] = RandomColor();
//			c[i+3] = RectColorCalc(c[i+0], c[i+1], c[i+2]);
		}
					
		for(size_t i = 0; i < mesh->getVertexes().size(); ++i)
		{
//			Color c(rand());
//			c.setAlpha(0xff);
			m_colors.push_back(c[i]);
		}
	}
	std::shared_ptr<Mesh> Entity::getMesh()
	{
		return m_mesh;
	}
	
	void Entity::draw()
	{
		#if ZEN_DRAW_GLES
		auto sha = GL::ShaderPrograms::Get()
		->getShaderC(false);

		auto pm = DrawStack::Get()->getTopMatrix4();
		GLHelper::PerformBlend(m_blend);

		auto s = GLHelper::GetGLRender();
		
//		s->enableDepthMask(false);
		s->activeProgram(sha->program.getID());

		r += 0.001f * ro;
//		move.z -= 0.05f;
		Matrix4 mat;
		
//		s->enableBuffer(m_RBuf.v.getID());
		mat = *pm;

		if(show)
		{
			auto v1 = Matrix4MultiplyAndProjectVector3(mat, move);
			printf("%f - %f %f %f\n", move.z, v1.x, v1.y, v1.z);
		}
		move.z += dz;
//		if((dz > 0.f && move.z > 0) || (dz < 0.f && move.z < -500)) dz = -dz;
		mat = Matrix4Multiply(mat, Matrix4MakeTranslation(move.x, move.y, move.z));
		mat = Matrix4Multiply(mat, Matrix4MakeRotation(r, 1, 1, 0));
		mat = Matrix4Multiply(mat, Matrix4MakeScale(size, size, size));//(r, 0, 1, 0));

		s->enableBuffer(0);
		s->setVertexData(sha->a_coord, 3, GL::eType::Float, false, 0, m_mesh->getVertexes().data());
//		s->setVertex(sha->a_color, 0.4f, 0.2f, 0.1f, 1.f);
		s->setVertexData(sha->a_color, 4, GL::eType::Float, true, 0, m_colors.data());

		s->setUniform(sha->u_color, m_color.red, m_color.green, m_color.blue, m_color.alpha);
		s->setUniform(sha->u_transform, mat);
		s->drawElements(GL::eVertexMode::Triangle, 0, m_mesh->getIndexes().size()-0,
						m_mesh->getIndexes().data());
		#endif
	}
	
	void Ground::draw()
	{
		#if ZEN_DRAW_GLES
		auto sha = GL::ShaderPrograms::Get()
		->getShaderC(false);

		auto pm = DrawStack::Get()->getTopMatrix4();
		GLHelper::PerformBlend(m_blend);

		auto s = GLHelper::GetGLRender();
		
//		s->enableDepthMask(false);
		s->activeProgram(sha->program.getID());
		
//		s->enableBuffer(m_RBuf.v.getID());
		auto mat = Matrix4MakeIdentity();

//		float vy = 0.2;
		#define vy 0.2
		float vertex[4][3] = {
			{-1, -vy, -1 }, { 1, -vy, -1 },
			{-1, -vy, 1 }, { 1, -vy, 1 }
		};
		Color4f cs[4] = {
			eColor::Grey,
			eColor::Grey,
			eColor::Grey,
			eColor::Grey,
		};
		Color4f m_color = eColor::White;
		s->enableBuffer(0);
		s->setVertexData(sha->a_coord, 3, GL::eType::Float, false, 0, vertex);
//		s->setVertex(sha->a_color, 0.4f, 0.2f, 0.1f, 1.f);
		s->setVertexData(sha->a_color, 4, GL::eType::Float, true, 0, cs);

		s->setUniform(sha->u_color, m_color.red, m_color.green, m_color.blue, m_color.alpha);
		s->setUniform(sha->u_transform, mat);
//		s->drawElements(GL::eVertexMode::Triangle, 0, m_mesh->getIndexes().size()-0,
//						m_mesh->getIndexes().data());
		s->drawArray(GL::eVertexMode::TriangleStrip, 0, 4);
		#endif
	}
}}
