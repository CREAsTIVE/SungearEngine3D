#include <glad.h>
#include <iostream>
#include <vector>
#include <map>
#include "../../../GLFW/deps/linmath.h"

using namespace std;

namespace Core3D {
	namespace GLC {
		class GLBuffer {
		private:
			GLuint _handler = 0;
			GLenum _type= GL_DYNAMIC_DRAW;
			
		public:
			GLBuffer() : GLBuffer(GL_ARRAY_BUFFER) {}
			GLBuffer(GLenum type);
			~GLBuffer();

			GLBuffer* Bind();
			GLBuffer* Data(GLsizeiptr size, const void* data, GLenum usage= GL_DYNAMIC_DRAW);
			GLBuffer* Clear();
		};


		class GLShader {
		private:
			GLuint _programHandler = 0;

			map<std::string, GLint> _uniforms = map<std::string, GLint>();
			map<std::string, GLint> _attribs = map<std::string, GLint>();
			GLShader* getUniforms();
			GLShader* getAttribs();
			GLint getLocation(string name) 
			{ 
				if (!ContainsUniform(name)) { throw exception("BRUH"); }
				return _uniforms[name];
			}
			static const GLsizei bufferSize = 16;
		public:
			GLShader();
			GLShader(string computeShaderCode);
			GLShader(string fragmentShaderCode, string vertexShaderCode);
			~GLShader();

			GLShader* Use() { glUseProgram(_programHandler); return this; }

			// ���������� �������� (��� ����� ������ ��������)
			static GLuint Compile(string code, GLenum type); 
			static shared_ptr<vector<GLuint>> Compile(vector<tuple<string, GLenum>>& arr);

			//�������� ��������� � �������� � ���� �������� 
			GLShader* BuildProgram(GLuint shader);
			GLShader* BuildProgram(vector<GLuint>& shaders);

			GLShader* LinkProgram() { glLinkProgram(_programHandler); return this; }

			GLint getAttribLocation(string name) { return _attribs[name]; }// ��������� ����� ���� ����������� ������� ���������� ����������

			static void FromFile(string fragmentSource, string vertexSource);


			bool ContainsUniform(string name) { return _uniforms.find(name)!=_uniforms.end(); }
			// ������ ��� �� ������ uniform-� ������ �����
			void uMat4x4(string name, mat4x4 value) { glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, (const GLfloat*)value); }

			bool ContainsAttrib(string name) { return _attribs.find(name) != _attribs.end(); }
			// ������ ���������
			// TODO: ������� ��������� ����������
			
		};


		class GLVertexArray {
		private:
			GLuint _handler = 0;
			GLShader* _program = nullptr;
			// <<name, Buffer, dataSize, dataCount, dataType>>
			// dataSize - ������ ������ �� ���� �������
			vector<tuple<string, GLBuffer*, unsigned int, unsigned int, GLenum>> _attribsData = vector<tuple<string, GLBuffer*, unsigned int, unsigned, GLenum>>();
		public:
			GLVertexArray() { glGenVertexArrays(1, &_handler); }
			GLVertexArray(GLShader* shader) : GLVertexArray() { SetProgram(shader); }
			~GLVertexArray() {};

			GLVertexArray* SetProgram(GLShader* program) { _program = program; return this;}
			GLVertexArray* Bind() { glBindVertexArray(_handler); return this; }


			GLVertexArray* PutAttrib(string name, GLBuffer* buffer, unsigned int dataSize, unsigned int dataCount, GLenum dataType) 
				{ _attribsData.push_back(make_tuple(name, buffer, dataSize, dataCount, dataType)); return this; }

			GLVertexArray* PutAttrib(string name, vector<float>* arr, int vertexDataCount)
				{ PutAttrib(name, (new GLBuffer())->Bind()->Data(sizeof(arr), arr), sizeof(float)*vertexDataCount, arr->size(), GL_FLOAT); }


			GLVertexArray* BuildAttribs();
		};
	}
}