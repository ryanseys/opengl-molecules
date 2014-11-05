all:
	g++ -w main.cpp ryan_camera.cpp Shader.cpp pugixml.cpp -o mol.out -framework OpenGL -framework GLUT && ./mol.out
