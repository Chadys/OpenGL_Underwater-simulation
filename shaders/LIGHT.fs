// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, Material mat, vec3 normal, vec3 viewDir, vec4 textures[6]){
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), mat.shininess);
    // Combine results
    vec3 ambient = light.ambient * textures[0].rgb;
    vec3 diffuse = light.diffuse * diff * textures[0].rgb;
    vec3 specular = light.specular * spec;
    if(mat.texture_specular){
        specular *= textures[1].rgb;
    }
    return (ambient + diffuse + specular);
}

vec3 CalcDirLight(DirLight light, float shininess, vec3 normal, vec3 viewDir, vec4 color){
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    // Combine results
    vec3 ambient = light.ambient * color.rgb;
    vec3 diffuse = light.diffuse * diff * color.rgb;
    vec3 specular = light.specular * spec;
    return (ambient + diffuse + specular);
}