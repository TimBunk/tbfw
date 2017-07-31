#version 440 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D emission;
  float shininess;
};

struct Light
{
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 fragPos;
in vec3 normal;
in vec2 textCoord;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform float visibility;

void main() {
  // ambient
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, textCoord));

  // diffuse
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(-light.direction);
  float diff = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, textCoord));

  // specular
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, textCoord));

  // final result
  vec3 result = (ambient + diffuse + specular);// * objectColor;
  color = vec4(result + (vec3(texture(material.emission, textCoord) * visibility)), 1.0f);
}
