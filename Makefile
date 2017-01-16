train:
	g++ main.cpp -std=c++14 -O3 -o train.out
draw:
	g++ draw_dataset.cpp -framework OpenGL -framework GLUT -Wno-deprecated-declarations -std=c++14 -o draw.out
test:
	g++ test.cpp -std=c++14 -o test.out
eval:
	g++ eval.cpp -std=c++14 eval.out
draw_w:
	g++ ./draw_weigths.cpp -framework OpenGL -framework GLUT -Wno-deprecated-declarations -std=c++14 -o draw_weigths.out
