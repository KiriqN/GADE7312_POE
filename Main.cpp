//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------
#include <ft2build.h>
#include FT_FREETYPE_H
#include"Model.h"




const unsigned int width = 1600;
const unsigned int height = 900;





float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,       
	 1.0f, -1.0f,  1.0f,       
	 1.0f, -1.0f, -1.0f,     
	-1.0f, -1.0f, -1.0f,      
	-1.0f,  1.0f,  1.0f,      
	 1.0f,  1.0f,  1.0f,      
	 1.0f,  1.0f, -1.0f,     
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};




int main()
{
	
	glfwInit();


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	GLFWwindow* window = glfwCreateWindow(width, height, "Window", NULL, NULL);
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);


	gladLoadGL();
	
	glViewport(0, 0, width, height);

	





	// store shaders for model and skybox
	Shader shaderProgram("default.vert", "default.frag");
	Shader skyboxShader("skybox.vert", "skybox.frag");

	// light color and position
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, -30.0f);

	glm::vec3 spotLight = glm::vec3(100.5f, 0.5f, -30.0f);
	//glm::vec3 lightDir = glm::vec3(10.5f, 10.5f, -30.0f);

	

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "spotLight"), lightPos.x, lightPos.y, lightPos.z);

	//glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);



	

	//all enables --
	glEnable(GL_DEPTH_TEST);

	
	glEnable(GL_CULL_FACE);
	
	glCullFace(GL_FRONT);
	
	glFrontFace(GL_CCW);
	





	// Creates camera object
	Camera camera(width, height, glm::vec3(0.1f, 0.7f, 3.0f));

	


	//get path for models
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string modelPath = "/Resources/All/model/Stanford Bunny/Queen.gltf";

	std::string parentDir2 = (fs::current_path().fs::path::parent_path()).string();
	std::string modelPath2 = "/Resources/All/model/Stanford Bunny/scene.gltf";

	std::string parentDir3 = (fs::current_path().fs::path::parent_path()).string();
	std::string modelPath3 = "/Resources/All/model/Stanford Bunny/Pawn.gltf";

	std::string parentDir4 = (fs::current_path().fs::path::parent_path()).string();
	std::string modelPath4 = "/Resources/All/model/Stanford Bunny/King.gltf";

	std::string parentDir5 = (fs::current_path().fs::path::parent_path()).string();
	std::string modelPath5 = "/Resources/All/model/Stanford Bunny/Board.gltf";
	

	


	
	// Load in model from path
	Model model((parentDir + modelPath).c_str());
	Model model2((parentDir2 + modelPath2).c_str());
	Model model3((parentDir3 + modelPath3).c_str());
	Model model4((parentDir4 + modelPath4).c_str());
	Model model5((parentDir5 + modelPath5).c_str());


	
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	// num frames that occur counter
	unsigned int counter = 0;




	// make VAO, VBO, and EBO for skybox cube
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	std::string facesCubemap[6] =
	{
		parentDir + "/Resources/All/skybox/right_x+1.jpg",
		parentDir + "/Resources/All/skybox/left_x-1.jpg",
		parentDir + "/Resources/All/skybox/top_y+1.jpg",
		parentDir + "/Resources/All/skybox/bottom_y-1.jpg",
		parentDir + "/Resources/All/skybox/front_z+1.jpg",
		parentDir + "/Resources/All/skybox/back_z-1.jpg"
	};

	// Creates the cubemap texture object
	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	// loop through each texture and map them to the cube
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}



	
	while (!glfwWindowShouldClose(window))
	{
		
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

		if (timeDiff >= 1.0 / 30.0)
		{
			// display game counter in window 
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "Window - " + FPS + "FPS / " + ms + "ms";
			
			glfwSetWindowTitle(window, newTitle.c_str());

			// resets times and counter
			prevTime = crntTime;
			counter = 0;

		
		}

		

		// specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// vlean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		camera.Inputs(window);
		// updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		

		// draw the chess model
		model.Draw(shaderProgram, camera);
		model2.Draw(shaderProgram, camera);
		model3.Draw(shaderProgram, camera);
		model4.Draw(shaderProgram, camera);
			
	    
		
		
		
		

		

		
		glDepthFunc(GL_LEQUAL);

		skyboxShader.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		//we make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
		//the last row and column affect the translation of the skybox (which we don't want to affect)
		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
		// were an object is present (a depth of 1.0f will always fail against any object's depth value)
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// switch back to the normal depth function
		glDepthFunc(GL_LESS);


		// swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// take care of all GLFW events
		glfwPollEvents();
	}







	// remove used system and terminate
	shaderProgram.Delete();
	skyboxShader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}




