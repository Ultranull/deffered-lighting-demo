#include "Material.h"




void Texture::load(){
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, FILTER);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, FILTER);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, WRAP);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, WRAP);
	glGenerateMipmap(target);

}

void Texture::setSize(int w, int h){
	width = w;
	height = h;
}

void Texture::bind(){
	glBindTexture(target, id);
}

GLint Texture::activate(GLenum texture) {
	glActiveTexture(texture);
	glBindTexture(GL_TEXTURE_2D, id);
	return texture - GL_TEXTURE0;
}

void Texture::cleanup(){
	glDeleteTextures(1, &id);
}

void Texture::unbind(GLuint target){
	glBindTexture(target, 0);
}
