#include "Application.h"
using namespace Engine;

std::shared_ptr<Application> application;

void motionFunc(int x, int y)
{
	application->motionFunc(x, y);
}

void processMouseClick(int button, int state, int x, int y)
{
	application->processMouseClick(button, state, x, y);
}

void keyboardInput(unsigned char c, int x, int y)
{
	application->keyboardInput(c, x, y);
}

void keyboardInputUp(unsigned char c, int x, int y)
{
	application->keyboardInputUp(c, x, y);
}

void display(void)
{
	application->render();
}

void resize(int width, int height)
{
	application->resize(width, height);
}

void specialKeyInput(int key, int x, int y)
{
	application->specialKeyInput(key, x, y);
}

void specialKeyInputUp(int key, int x, int y)
{
	application->specialKeyInputUp(key, x, y);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(400, 300);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Space Shooter");

	glewExperimental = true;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		#if _DEBUG
			std::cout << "ERROR::GLEW: " << glewGetErrorString(err) << std::endl;
		#endif
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutIgnoreKeyRepeat(GL_TRUE);

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboardInput);
	glutKeyboardUpFunc(keyboardInputUp);
	glutMouseFunc(processMouseClick);
	glutPassiveMotionFunc(motionFunc);
	glutSpecialFunc(specialKeyInput);
	glutSpecialUpFunc(specialKeyInputUp);

	application = std::make_shared<Application>();

	glClearColor(52.0f / 255.0f, 40.0f / 255.0f, 44.0f / 255.0f, 1.0f);
	glutMainLoop();
	return EXIT_SUCCESS;
}
