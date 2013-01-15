#pragma comment(lib, "GLFWDLL")
#pragma comment(lib, "OpenGL32")
#pragma comment(lib, "glew32")
#pragma comment(lib, "DevIL")
#pragma comment(lib, "ILU")
#pragma comment(lib, "ILUT")

#define GLFW_DLL

#include "NeededHeaders.h"
#include "FrustrumPlanes.h"
#include "ViewingFrustrum.h"
#include "Planes.h"

bool running = true;

effect eff;

scene_data* scene;
scene_data* scene1;
scene_data* scene2;
scene_data* scene3;

first_person_camera* fpsCam;
first_person_camera* cam;
camera* currentCam;
frustrumPlanes* planes;
viewingFrustrum* viewFrus; 
frus* frustum;

//float screenHeight = 600.0f;
//float screenWidth = 800.0f;
float screenHeight = 1080.0f;
float screenWidth = 1920.0f;

glm::vec4 fogColour = glm::vec4(0.7, 0.7, 0.7, 1.0);
GLfloat fogDensity = 0.01f;
GLfloat fogOn = 1.0f;

glm::vec3 lightAim(-2.5f, 0.0f, -1.5f);

void initialise()
{
	srand(time(NULL));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);
	glClearColor(0.3f, 0.6f, 0.9f, 1.0f);

	//========Set Camera=====================================
	fpsCam = new first_person_camera();
	fpsCam->setProjection(glm::pi<float>() / 4.0f, screenWidth/screenHeight, 0.1f, 10000.0f);
	fpsCam->setPositon(glm::vec3(1.73964, 4.51647, -23.0497));
	fpsCam->rotate(0.0f, 0.0f);
	currentCam = fpsCam;
	//=============================================================

	//Setup frustrum planes
	planes = new frustrumPlanes();
	planes->setPlanes(currentCam);
	viewFrus = new viewingFrustrum(); 
	frustum = new frus();

	//==========Set up shaders================================ 
	if (!eff.addShader("lit_textured.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("lighting.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("point_light.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("spot_light.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("Fog.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("lit_textured.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.create())
		exit(EXIT_FAILURE);	
	//==========================================================

	//==========Load in scene file=============================
	scene1 = loadScene("TableAndChairs.json");
	scene = scene1;
	//=========================================================

	//=========Create terrain===================================
	geometry* geom = createTerrain(scene1->textures["heightmap"]);
	scene1->geometry["terrain"] = geom;

	render_object* obj = new render_object();
	obj->geometry = geom;
	obj->material = scene1->material["mat_001"];
	obj->transform.scale = (glm::vec3(1.0, 8.0, 1.0));
	obj->transform.position = (glm::vec3(0.0, -3.0, 0.0));
	scene1->objects["terrain"] = obj;
	//==========================================================
}

void moveFPSCam(float deltaTime, float speed)
{
	cam = (first_person_camera*)currentCam;
	//fps cam controls
	if (glfwGetKey('W'))
		cam->move(glm::vec3(0.0f, 0.0f, speed) * (float)deltaTime);
	if (glfwGetKey('S'))
		cam->move(-glm::vec3(0.0f, 0.0f, speed) * (float)deltaTime);
	if (glfwGetKey('A'))
		cam->move(glm::vec3(speed, 0.0f, 0.0f) * (float)deltaTime);
	if (glfwGetKey('D'))
		cam->move(-glm::vec3(speed, 0.0f, 0.0f) * (float)deltaTime);
	if (glfwGetKey('E'))
		cam->rotate(-glm::pi<float>() / 4.0f * deltaTime, 0.0f);
	if (glfwGetKey('Q'))
		cam->rotate(glm::pi<float>() / 4.0f * deltaTime, 0.0f);
	if (glfwGetKey(GLFW_KEY_LSHIFT))
		cam->move(-glm::vec3(0.0f, speed, 0.0f) * (float)deltaTime);
	if (glfwGetKey(GLFW_KEY_SPACE))
		cam->move(glm::vec3(0.0f, speed, 0.0f) * (float)deltaTime);
}

void render(const effect* eff, const glm::mat4 view, const glm::mat4& projection, const render_object* object)
{
	glm::mat4 mvp = projection * view * object->transform.getTransformationMatrix();
	glUniformMatrix4fv(eff->getUniformIndex("modelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));
	glm::mat4 mit = glm::inverse(glm::transpose(object->transform.getTransformationMatrix()));
	glUniformMatrix4fv(eff->getUniformIndex("modelInverseTranspose"), 1, GL_FALSE, glm::value_ptr(mit));
	glUniformMatrix4fv(eff->getUniformIndex("model"), 1, GL_FALSE, glm::value_ptr(object->transform.getTransformationMatrix()));
	CHECK_GL_ERROR
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), object->transform.scale);
	glUniformMatrix4fv(eff->getUniformIndex("scale"), 1, GL_FALSE, glm::value_ptr(scale));

	CHECK_GL_ERROR
	object->material->bind(eff);

	glBindVertexArray(object->geometry->vao);
	if (object->geometry->indexBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->geometry->indexBuffer);
		glDrawElements(GL_TRIANGLES, object->geometry->indices.size(), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		CHECK_GL_ERROR
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, object->geometry->vertices.size());
	glBindVertexArray(0);
	CHECK_GL_ERROR
}

void render()
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	eff.begin();
	scene->light.bind(&eff);
	scene->dynamic.bind(&eff);
	
	glUniform3fv(eff.getUniformIndex("eyePos"), 1, glm::value_ptr(currentCam->getPosition()));
	glUniform4fv(eff.getUniformIndex("fogCol"), 1, glm::value_ptr(fogColour));
	glUniform1fv(eff.getUniformIndex("density"), 1, &fogDensity);
	glUniform1fv(eff.getUniformIndex("fogOn"), 1, &fogOn);
	
	CHECK_GL_ERROR
	
	std::hash_map<std::string, render_object*>::const_iterator iter = scene->objects.begin();
	for (; iter != scene->objects.end(); ++iter)
	{
		if (planes->checkPointValid(iter->second->transform.position, iter->second->transform.getTransformationMatrix(), currentCam))
			render(&eff, currentCam->getView(), currentCam->getProjecion(), iter->second);
	}

	eff.end();
	glUseProgram(0);
	CHECK_GL_ERROR
	glfwSwapBuffers();
}

void update(double deltaTime)
{
	
	currentCam->update(deltaTime);
	running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	moveFPSCam(deltaTime, 5.0f);

	float speed = 0.01;

	//Change the colour of the fog on key press
	if (glfwGetKey('1'))
		fogColour.x += speed;
	if (glfwGetKey('2'))
		fogColour.y += speed;
	if (glfwGetKey('3'))
		fogColour.z += speed;
	if (glfwGetKey('4'))
		fogColour.x -= speed;
	if (glfwGetKey('5'))
		fogColour.y -= speed;
	if (glfwGetKey('6'))
		fogColour.z -= speed;

	//Chnage the density of the fog on key press
	if (glfwGetKey('P'))
		fogDensity += 0.0005;
	if (glfwGetKey('O'))
		fogDensity -= 0.0005;

	//Switch fog on or off
	if (glfwGetKey('I'))
			fogOn = 0.0f;
	if (glfwGetKey('U'))
			fogOn = 1.0f;	

	//Switch wireframe or or off
	if (glfwGetKey('M'))
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	if (glfwGetKey('N'))
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void cleanup()
{
}

void frustrumDebugStatements()
{
	std::cout<<"NearPlane = " << planes->getNearPlane().x <<" , " << planes->getNearPlane().y <<" , " << planes->getNearPlane().z<<" , " << planes->getNearPlane().w<<std::endl;
		std::cout<<"FarPlane = " << planes->getFarPlane().x <<" , " << planes->getFarPlane().y <<" , " << planes->getFarPlane().z<<" , " << planes->getFarPlane().w<<std::endl;
		std::cout<<"LeftPlane = " << planes->getLeftPlane().x <<" , " << planes->getLeftPlane().y <<" , " << planes->getLeftPlane().z<<" , " << planes->getLeftPlane().w<<std::endl;
		std::cout<<"RightPlane = " << planes->getRightPlane().x <<" , " << planes->getRightPlane().y <<" , " << planes->getRightPlane().z<<" , " << planes->getRightPlane().w<<std::endl;
		std::cout<<"TopPlane = " << planes->getTopPlane().x <<" , " << planes->getTopPlane().y <<" , " << planes->getTopPlane().z<<" , " << planes->getTopPlane().w<<std::endl;
		std::cout<<"BottomPlane = " << planes->getBottomPlane().x <<" , " << planes->getBottomPlane().y <<" , " << planes->getBottomPlane().z<<" , " << planes->getBottomPlane().w<<std::endl;
}

void checkPoints()
{
	//glm::vec3 point = glm::vec3(scene->objects["topTable"]->transform.position);
	//std::cout << "topTablePos.x  = " << point.x << std::endl;
	//bool test = planes->checkPointValid(point, scene->objects["topTable"]->transform.getTransformationMatrix());
	
	//std::vector<glm::vec3> verts = scene->objects["topTable"]->geometry->vertices;
	//glm::vec3 point;
	//for (std::vector<glm::vec3>::iterator iter = verts.begin(); iter != verts.end(); ++iter)
	//{
	//	point.x = iter->x; point.y = iter->y, point.z = iter->z;
	//	if (planes->checkPointValid(point, scene->objects["topTable"]->transform.getTransformationMatrix()))
	//	{
	//		//std::cout <<"Point: " << point.x << " , "<< point.y << " , "<< point.z<<" Is valid"<<std::endl; 
	//	}
	//	else
	//		int y = 1;
	//		//std::cout <<"Point: " << point.x << " , "<< point.y << " , "<< point.z<<" NOT VALID"<<std::endl;
	//}	

	//if (planes->checkPointValid(glm::vec3(20.0f, 100.0f, -50.0f), scene->objects["topTable"]->transform.getTransformationMatrix()))
	//{

	//}
}

void checkPoints2()
{

	glm::vec3 point = glm::vec3(10.0f, 15.0f, -20.0f);
	viewFrus->checkPoint(point);
	
}

//void checkPoints3()
//{
//	glm::vec3 point = glm::vec3(0.0f, 0.0f, 0.0f);
//	frustum->checkPoint(point, scene->objects["topTable"]->transform.getTransformationMatrix());
//}

int main()
{
	SET_DEBUG

	if (!glfwInit())
		exit(EXIT_FAILURE);
	if (!glfwOpenWindow(screenWidth, screenHeight, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::cout << "Error: " << glewGetErrorString(error) <<std::endl;
		exit(EXIT_FAILURE);
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major , minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("GL VENDOR : %s\n", vendor);
	printf("GL Renderer : %s\n", renderer);
	printf("GL Version (string) : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL version : %s\n", glslVersion);

	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	initialise();
	frustrumDebugStatements();


	double prevTimeStamp = glfwGetTime();
	double currentTimeStamp;
	while (running)
	{
		currentTimeStamp = glfwGetTime();
		update(currentTimeStamp - prevTimeStamp);
		
		render();
		//checkPoints();

		prevTimeStamp = currentTimeStamp;
	}

	cleanup();

	glfwTerminate();

	exit(EXIT_SUCCESS);
}