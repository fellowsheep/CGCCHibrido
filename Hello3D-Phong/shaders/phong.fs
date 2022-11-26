#version 450 core

out vec4 color;

//Informações provenientes do vertex shader
in vec3 scaledNormal; //vetor normal com correção de escala
in vec3 fragPos; //posição em coord de mundo do objeto

//Informações provenientes do código em C++
uniform vec3 lightPos;      //posiçao da fonte de luz 
uniform vec3 viewPos;       //posicao da camera
uniform vec3 lightColor;    //cor da fonte de luz
uniform vec3 objectColor;   //cor do objeto 
uniform float ka;           //coeficiente de reflexão ambiente
uniform float kd;           //coeficiente de reflexão difuse
uniform float ks;           //coeficiente de reflexão especular
uniform float n;            //expoente da reflexão especular

void main()
{
    // Ambient
    float ambientStrength = 1.0f;
    vec3 ambient = ambientStrength * lightColor * ka;
  	
    // Diffuse 
    vec3 norm = normalize(scaledNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * kd;
    
    // Specular
    float specularStrength = 1.0f;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), n);
    vec3 specular = spec * lightColor * ks;  
      
    //vec3 result = (ambient + diffuse) * objectColor;
    vec3 result = (ambient + diffuse) * objectColor + specular;
	
    color = vec4(result, 1.0f);
    //color = vec4(objectColor,1);
}
    