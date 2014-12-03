// temporary file for the textures

/**************************************************************************************/
//  Define the geometry


void loadSkyboxTexturesCat(GLuint *tex, char *imageFileName) {
  int width=1, height=1;
  unsigned char* image;

  // int channels;
  // image = SOIL_load_image(imageFileName, &width, &height, 0, SOIL_LOAD_RGB);
  // *tex = SOIL_load_OGL_texture(imageFileName, SOIL_LOAD_AUTO,  SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  // SOIL_free_image_data(image);

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // generate the texture

  // colors for sides of cube
  GLubyte posX[4] = {255, 0, 0, 1};
  GLubyte negX[4] = {0, 255, 0, 1};
  GLubyte posY[4] = {0, 0, 255, 1};
  GLubyte negY[4] = {0, 255, 255, 1};
  GLubyte posZ[4] = {255, 0, 255, 1};
  GLubyte negZ[4] = {255, 255, 0, 1};


  //glEnable(GL_TEXTURE_CUBE_MAP);
  glActiveTexture(GL_TEXTURE3);

  // Create texture object
  glGenTextures(1, tex);
  //glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, *tex);
  checkError();

  image = SOIL_load_image("sample.png", &width, &height, 0, SOIL_LOAD_RGB);
  //*tex = SOIL_load_OGL_texture(imageFileName, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  checkError();

    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  checkError();

  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, image);
  checkError();

  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, image);
  checkError();

  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, image);
  checkError();

  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
  checkError();


  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  checkError();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
  checkError();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
  checkError();
}

/**************************************************************************************/
//  Define the geometry
void loadSkyboxTexturesSky(GLuint *tex, char *imageFileName) {

  int width=1, height=1;
  unsigned char* image;

  // int channels;

  //image = SOIL_load_image(imageFileName, &width, &height, 0, SOIL_LOAD_RGB);
  //*tex = SOIL_load_OGL_texture(imageFileName, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  //SOIL_free_image_data(image);

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // generate the texture

  // colors for sides of cube
  GLubyte posX[4] = {255, 0, 0, 1};
  GLubyte negX[4] = {0, 255, 0, 1};
  GLubyte posY[4] = {0, 0, 255, 1};
  GLubyte negY[4] = {0, 255, 255, 1};
  GLubyte posZ[4] = {255, 0, 255, 1};
  GLubyte negZ[4] = {255, 255, 0, 1};

  //glEnable(GL_TEXTURE_CUBE_MAP);
  glActiveTexture(GL_TEXTURE3);

  // Create texture object
  glGenTextures(1, tex);
  //glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, *tex);
  checkError();

  image = SOIL_load_image("TropicalSunnyDayRight2048.png", &width, &height, 0, SOIL_LOAD_RGB);
  //*tex = SOIL_load_OGL_texture(imageFileName, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);
  checkError();

  image = SOIL_load_image("TropicalSunnyDayLeft2048.png", &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);
  checkError();

  image = SOIL_load_image("TropicalSunnyDayUp2048.png", &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);
  checkError();

  image = SOIL_load_image("TropicalSunnyDayDown2048.png", &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, image);
  checkError();
  SOIL_free_image_data(image);

  image = SOIL_load_image("TropicalSunnyDayFront2048.png", &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);
  checkError();

  image = SOIL_load_image("TropicalSunnyDayBack2048.png", &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);
  checkError();

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  checkError();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
  checkError();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
  checkError();
}

//  Define the geometry
void loadSkyboxTexturesColour(GLuint *tex, char *imageFileName) {
  int width=1, height=1;
  // unsigned char* image;
  // int channels;

  //image = SOIL_load_image(imageFileName, &width, &height, 0, SOIL_LOAD_RGB);
  //*tex = SOIL_load_OGL_texture(imageFileName, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  //SOIL_free_image_data(image);

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // generate the texture

  // colors for sides of cube
  GLubyte posX[4] = {255, 0, 0, 1};
  GLubyte negX[4] = {0, 255, 0, 1};
  GLubyte posY[4] = {0, 0, 255, 1};
  GLubyte negY[4] = {0, 255, 255, 1};
  GLubyte posZ[4] = {255, 0, 255, 1};
  GLubyte negZ[4] = {255, 255, 0, 1};

  //glEnable(GL_TEXTURE_CUBE_MAP);
  glActiveTexture(GL_TEXTURE3);

  // Create texture object
  glGenTextures(1, tex);
  //glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, *tex);
  checkError();

  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, posX);
  checkError();
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, negX);
  checkError();
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, posY);
  checkError();
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, negY);
  checkError();
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, posZ);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, negZ);
  checkError();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  checkError();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
  checkError();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
  checkError();
}

void fillArray(GLubyte *image, int size, GLubyte *colour, int colourSize) {
  int i, j;
  int add;  // address of curent colour

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      add = (i*size + j) * colourSize;
      memcpy(&image[add], colour, colourSize);
    }
  }
}

//  Define the geometry
void loadSkyboxTexturesColour32(GLuint *tex, char *imageFileName) {
  int width=1, height=1;
  // unsigned char* image;
  // int channels;

  //image = SOIL_load_image(imageFileName, &width, &height, 0, SOIL_LOAD_RGB);
  //*tex = SOIL_load_OGL_texture(imageFileName, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  //SOIL_free_image_data(image);

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // generate the texture

  // colors for sides of cube
  GLubyte posX[4] = {255, 0, 0, 1};
  GLubyte negX[4] = {0, 255, 0, 1};
  GLubyte posY[4] = {0, 0, 255, 1};
  GLubyte negY[4] = {0, 255, 255, 1};
  GLubyte posZ[4] = {255, 0, 255, 1};
  GLubyte negZ[4] = {255, 255, 0, 1};

  GLubyte red[3] = {255, 0, 0};
  GLubyte green[3] = {0, 255, 0};
  GLubyte blue[3] = {0, 0, 255};
  GLubyte yellow[3] = {0, 255, 255};
  GLubyte magenta[3] = {255, 0, 255};
  GLubyte cyan[3] = {255, 255, 0};

  GLubyte image[32][32][3];
  width = height = 32;

  //glEnable(GL_TEXTURE_CUBE_MAP);
  glActiveTexture(GL_TEXTURE3);

  // Create texture object
  glGenTextures(1, tex);
  //glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, *tex);
  checkError();

  fillArray((GLubyte *) image, 32, red, sizeof(red));
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, image);
  checkError();

  fillArray((GLubyte *) image, 32, green, sizeof(green));
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, image);
  checkError();

  fillArray((GLubyte *) image, 32, blue, sizeof(blue));
  fillArray((GLubyte *) image, 32, red, sizeof(red));
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, image);
  checkError();

  fillArray((GLubyte *) image, 32, yellow, sizeof(yellow));
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, image);
  checkError();

  fillArray((GLubyte *) image, 32, magenta, sizeof(magenta));
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, image);
  checkError();

  fillArray((GLubyte *) image, 32, cyan, sizeof(cyan));
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, image);
  checkError();

  /***************************************
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, image);
  checkError();

  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, image);
  checkError();

  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, image);
  checkError();

  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, image);
  checkError();

  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, image);
  checkError();

  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, image);
  checkError();
  */

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  checkError();

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
  checkError();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
  checkError();
}
