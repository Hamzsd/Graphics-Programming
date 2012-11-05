#include "scene.h"

#include <boost\property_tree\json_parser.hpp>
#include <IL\ilut.h>

glm::vec3 readVec3(const boost::property_tree::ptree& pt)
{
	glm::vec3 v;
	boost::property_tree::ptree::const_iterator iter = pt.begin();
	v.x = (iter++)->second.get_value<float>();
	v.y = (iter++)->second.get_value<float>();
	v.z = (iter++)->second.get_value<float>();
	return v;
}

glm::vec4 readVec4(const boost::property_tree::ptree& pt)
{
	glm::vec4 v;
	boost::property_tree::ptree::const_iterator iter = pt.begin();
	v.x = (iter++)->second.get_value<float>();
	v.y = (iter++)->second.get_value<float>();
	v.z = (iter++)->second.get_value<float>();
	v.w = (iter++)->second.get_value<float>();
	return v;
}

void readGeometry(scene_data* scene, const boost::property_tree::ptree& pt)
{
	boost::property_tree::ptree::const_iterator iter = pt.begin();
	for (; iter != pt.end(); ++iter)
	{
		std::string name = iter->first;
		geometry* geom;
		if (name == "cube")
			geom = createBox();
		else if (name == "tetrahedron")
			geom = createTetrahedron();
		else if (name == "pyramid")
			geom = createPyramid();
		else if (name == "disk")
		{
			int slices = iter->second.get_child("slices").get_value<int>();
			geom = createDisk(slices);
		}
		else if (name == "cylinder")
		{
			int slices = iter->second.get_child("slices").get_value<int>();
			int stacks = iter->second.get_child("stacks").get_value<int>();
			geom = createCylinder(stacks, slices);
		}
		else if (name == "sphere")
		{
			int slices = iter->second.get_child("slices").get_value<int>();
			int stacks = iter->second.get_child("stacks").get_value<int>();
			geom = createSphere(stacks, slices);
		}
		else if (name =="divSphere")
		{
			int divisions = iter->second.get_child("divisions").get_value<int>();
			geom = createSphere(divisions);
		}
		else if (name == "torus")
		{
			float radius = iter->second.get_child("radius").get_value<float>();
			int slices = iter->second.get_child("slices").get_value<int>();
			int stacks = iter->second.get_child("stacks").get_value<int>();
			geom = createTorus(radius, stacks, slices);
		}
		else if (name == "plane")
		{
			int width = iter->second.get_child("width").get_value<int>();
			int depth = iter->second.get_child("depth").get_value<int>();
			geom = createPlane(width, depth);
		}
		else if (name == "sierpinski")
		{
			int divisions = iter->second.get_child("divisions").get_value<int>();
			geom = createSierpinski(divisions);
		}
		else
		{
			std::cerr << "Error - Geometry type not recognised: " << name << std::endl;
			exit(EXIT_FAILURE);
		}
		scene->geometry[name] = geom;
	}
}

void readTextures(scene_data* scene, const boost::property_tree::ptree& pt)
{
	boost::property_tree::ptree::const_iterator iter = pt.begin();
	for (; iter != pt.end(); ++iter)
	{
		std::string name = iter->first;
		std::string filename = iter->second.get_value<std::string>();
		texture* tex = new texture(filename);
		if (!tex->create())
			printf("Error loading texture: 5s\n", filename);
		else
			scene->textures[name] = tex;
	}
}

void readMaterials(scene_data* scene, const boost::property_tree::ptree& pt)
{
	boost::property_tree::ptree::const_iterator iter = pt.begin();
	for (; iter != pt.end(); ++iter)
	{
		material* mat = new material();
		std::string name = iter->first;
		mat->data.emissive = readVec4(iter->second.get_child("emmisive"));
		mat->data.ambient = readVec4(iter->second.get_child("ambient"));
		mat->data.diffuse = readVec4(iter->second.get_child("diffuse"));
		mat->data.specular = readVec4(iter->second.get_child("specular"));
		mat->data.shininess = iter->second.get_child("shininess").get_value<float>();
		std::string texture = iter->second.get_child("texture").get_value<std::string>();
		mat->texture = scene->textures[texture];
		mat->create();
		scene->material[name] = mat;
	}
}

void readTransform(render_object* object, const boost::property_tree::ptree& pt)
{
	object->transform.position = readVec3(pt.get_child("position"));
	glm::vec3 rot = readVec3(pt.get_child("rotation"));
	object->transform.rotate(rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
	object->transform.rotate(rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
	object->transform.rotate(rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
	object->transform.scale = readVec3(pt.get_child("scale"));
}

void readObjects(scene_data* scene, const boost::property_tree::ptree& pt)
{
	boost::property_tree::ptree::const_iterator iter = pt.begin();
	for (; iter != pt.end(); ++iter)
	{
		render_object* object = new render_object();
		std::string name = iter->first;
		std::string geom = iter->second.get_child("geometry").get_value<std::string>();
		object->geometry = scene->geometry[geom];
		std::string mat = iter->second.get_child("material").get_value<std::string>();
		object->material = scene->material[mat];
		readTransform(object, iter->second.get_child("transform"));
		scene->objects[name] = object;
	}
}

void readLighting(scene_data* scene, const boost::property_tree::ptree& pt)
{
	scene->light.data.ambient = readVec4(pt.get_child("ambient"));
	scene->light.data.diffuse = readVec4(pt.get_child("diffuse"));
	scene->light.data.specular = readVec4(pt.get_child("specular"));
	scene->light.data.lightDir = readVec4(pt.get_child("lightDir"));
	scene->light.create();
}

void readPointLight(scene_data* scene, const boost::property_tree::ptree& pt)
{
	point_light_data point;
	point.ambient = readVec4(pt.get_child("ambient"));
	point.diffuse = readVec4(pt.get_child("diffuse"));
	point.specular = readVec4(pt.get_child("specular"));
	point.lightPos = readVec4(pt.get_child("lightPos"));
	point.attenuation = readVec4(pt.get_child("attenuation"));
	point.dist = pt.get_child("dist").get_value<float>();
	scene->dynamic.addPoint(point);
}

void readSpotLight(scene_data* scene, const boost::property_tree::ptree& pt)
{
	spot_light_data spot;
	spot.ambient = readVec4(pt.get_child("ambient"));
	spot.diffuse = readVec4(pt.get_child("diffuse"));
	spot.specular = readVec4(pt.get_child("specular"));
	spot.lightPos = readVec4(pt.get_child("lightPos"));
	spot.attenuation = readVec4(pt.get_child("attenuation"));
	spot.lightDir = readVec4(pt.get_child("lightDir"));
	spot.power = pt.get_child("power").get_value<float>();
	spot.dist = pt.get_child("dist").get_value<float>();
	scene->dynamic.addSpot(spot);
}

void readDynamicLights(scene_data* scene, const boost::property_tree::ptree& pt)
{
	boost::property_tree::ptree::const_iterator iter = pt.front().second.begin();
	for (; iter != pt.front().second.end(); ++iter)
	{
		if (iter->first == "point")
			readPointLight(scene, iter->second);
		else if (iter->first == "spot")
			readSpotLight(scene, iter->second);
		else
		{
			std::cerr << "Error - unknown dynamic light encountered: " << iter->second.front().first << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	scene->dynamic.create();
}

scene_data* loadScene(const std::string& fileName)
{
	scene_data* scene = new scene_data;

	boost::property_tree::ptree pt;
	boost::property_tree::read_json(fileName, pt);

	readGeometry(scene, pt.get_child("geometry"));
	readTextures(scene, pt.get_child("textures"));
	readMaterials(scene, pt.get_child("materials"));
	readObjects(scene, pt.get_child("objects"));
	readLighting(scene, pt.get_child("lighting"));
	readDynamicLights(scene, pt.get_child("dynamic_lighting"));

	return scene;
}

