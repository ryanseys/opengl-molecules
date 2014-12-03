all:
	g++ -I/opt/local/include/SOIL -L/opt/local/lib -w main.cpp ryan_camera.cpp Shader.cpp SkyBox.cpp pugixml.cpp -o mol.out -lSOIL -framework OpenGL -framework GLUT && ./mol.out
