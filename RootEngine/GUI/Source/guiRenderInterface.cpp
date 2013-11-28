
#include <Rocket/Core/Platform.h>
#include <windows.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "guiRenderInterface.h"
#include <RootEngine/Include/SubsystemSharedContext.h>

#define GL_CLAMP_TO_EDGE 0x812F


namespace RootEngine
{
	namespace GUISystem
	{
		extern RootEngine::SubsystemSharedContext g_context;
	}
}

guiRenderInterface::guiRenderInterface(void)
{
}


guiRenderInterface::~guiRenderInterface(void)
{
}

void guiRenderInterface::SetViewport(int width, int height)
{
    m_width = width;
    m_height = height;

	Rocket::Core::byte byteWhite[] = {255, 255, 255, 255};
	GenerateTexture(m_whiteTexture, byteWhite, Rocket::Core::Vector2i(1, 1));
}

void guiRenderInterface::SetEffect(Render::EffectInterface* p_effect)
{
	m_effect = p_effect;
	m_effect->Apply();
	m_effect->SetUniformMatrix(
		"projectionMatrix",
		glm::mat4(
			2.f/m_width, 0.f, 0.f, 0.f,
			0.f, -2.f/m_height, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			-1.f, 1.f, 0.f, 1.f
		)
	);
}


// Called by Rocket when it wants to render geometry that it does not wish to optimise.
void guiRenderInterface::RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, const Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation)
{
	glm::mat4 modelMatrix = glm::translate(translation.x*2.f/m_width, translation.y*2.f/m_height, 0.f);

	m_effect->Apply();
	m_effect->SetUniformMatrix("modelMatrix", modelMatrix);

	std::vector<float> tmp;
	for( int i = 0; i < num_vertices; i++ )
	{
		tmp.push_back(vertices[i].position.x);
		tmp.push_back(vertices[i].position.y);
		
		tmp.push_back(vertices[i].colour.red/255.f);
		tmp.push_back(vertices[i].colour.green/255.f);
		tmp.push_back(vertices[i].colour.blue/255.f);
		tmp.push_back(vertices[i].colour.alpha/255.f);

		tmp.push_back(vertices[i].tex_coord.x);
		tmp.push_back(vertices[i].tex_coord.y);
	}

	GLuint gbuf[2], vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, gbuf);
	glBindBuffer(GL_ARRAY_BUFFER, gbuf[0]);
	glBufferData(GL_ARRAY_BUFFER, tmp.size() * sizeof(float), tmp.data(), GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char*)NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char*)NULL+2*sizeof(float));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (char*)NULL+6*sizeof(float));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gbuf[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(int), indices, GL_STREAM_DRAW);

	if(texture)
	{
		glActiveTexture(GL_TEXTURE0);
		m_effect->SetUniformInt("texSampler", 0);
		glBindTexture(GL_TEXTURE_2D, (GLuint) texture);

	}
	else
	{
		glActiveTexture(GL_TEXTURE0);
		m_effect->SetUniformInt("texSampler", 0);
		glBindTexture(GL_TEXTURE_2D, (GLuint) m_whiteTexture);
	}

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDisable( GL_CULL_FACE );
	glDisable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable( GL_CULL_FACE );
	glDisable( GL_BLEND );
	glDeleteBuffers(2, gbuf);
	glDeleteVertexArrays(1, &vao);
}

// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.		
Rocket::Core::CompiledGeometryHandle guiRenderInterface::CompileGeometry(Rocket::Core::Vertex* ROCKET_UNUSED(vertices), int ROCKET_UNUSED(num_vertices), int* ROCKET_UNUSED(indices), int ROCKET_UNUSED(num_indices), const Rocket::Core::TextureHandle ROCKET_UNUSED(texture))
{
	return (Rocket::Core::CompiledGeometryHandle) NULL;
}

// Called by Rocket when it wants to render application-compiled geometry.		
void guiRenderInterface::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle ROCKET_UNUSED(geometry), const Rocket::Core::Vector2f& ROCKET_UNUSED(translation))
{
}

// Called by Rocket when it wants to release application-compiled geometry.		
void guiRenderInterface::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle ROCKET_UNUSED(geometry))
{
}

// Called by Rocket when it wants to enable or disable scissoring to clip content.		
void guiRenderInterface::EnableScissorRegion(bool enable)
{
	if (enable)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

// Called by Rocket when it wants to change the scissor region.		
void guiRenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
	glScissor(x, m_height - (y + height), width, height);
}

// Set to byte packing, or the compiler will expand our struct, which means it won't read correctly from file
#pragma pack(1) 
struct TGAHeader 
{
	char  idLength;
	char  colourMapType;
	char  dataType;
	short int colourMapOrigin;
	short int colourMapLength;
	char  colourMapDepth;
	short int xOrigin;
	short int yOrigin;
	short int width;
	short int height;
	char  bitsPerPixel;
	char  imageDescriptor;
};
// Restore packing
#pragma pack()

// Called by Rocket when a texture is required by the library.		
bool guiRenderInterface::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
	Rocket::Core::FileInterface* file_interface = Rocket::Core::GetFileInterface();
	Rocket::Core::FileHandle file_handle = file_interface->Open(source);
	if (!file_handle)
	{
		return false;
	}
	
	file_interface->Seek(file_handle, 0, SEEK_END);
	size_t buffer_size = file_interface->Tell(file_handle);
	file_interface->Seek(file_handle, 0, SEEK_SET);
	
	char* buffer = new char[buffer_size];
	file_interface->Read(buffer, buffer_size, file_handle);
	file_interface->Close(file_handle);

	TGAHeader header;
	memcpy(&header, buffer, sizeof(TGAHeader));
	
	int color_mode = header.bitsPerPixel / 8;
	int image_size = header.width * header.height * 4; // We always make 32bit textures 
	
	if (header.dataType != 2)
	{
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24/32bit uncompressed TGAs are supported.");
		return false;
	}
	
	// Ensure we have at least 3 colors
	if (color_mode < 3)
	{
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24 and 32bit textures are supported");
		return false;
	}
	
	const char* image_src = buffer + sizeof(TGAHeader);
	unsigned char* image_dest = new unsigned char[image_size];
	
	// Targa is BGR, swap to RGB and flip Y axis
	for (long y = 0; y < header.height; y++)
	{
		long read_index = y * header.width * color_mode;
		long write_index = ((header.imageDescriptor & 32) != 0) ? read_index : (header.height - y - 1) * header.width * color_mode;
		for (long x = 0; x < header.width; x++)
		{
			image_dest[write_index] = image_src[read_index+2];
			image_dest[write_index+1] = image_src[read_index+1];
			image_dest[write_index+2] = image_src[read_index];
			if (color_mode == 4)
				image_dest[write_index+3] = image_src[read_index+3];
			else
				image_dest[write_index+3] = 255;
			
			write_index += 4;
			read_index += color_mode;
		}
	}

	texture_dimensions.x = header.width;
	texture_dimensions.y = header.height;
	
	bool success = GenerateTexture(texture_handle, image_dest, texture_dimensions);
	
	delete [] image_dest;
	delete [] buffer;
	
	return success;
}

// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
bool guiRenderInterface::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
	GLuint texture_id = 0;
	glGenTextures(1, &texture_id);
	if (texture_id == 0)
	{
		printf("Failed to generate textures\n");
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, source_dimensions.x, source_dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	texture_handle = (Rocket::Core::TextureHandle) texture_id;

	return true;
}

// Called by Rocket when a loaded texture is no longer required.		
void guiRenderInterface::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
	glDeleteTextures(1, (GLuint*) &texture_handle);
}

