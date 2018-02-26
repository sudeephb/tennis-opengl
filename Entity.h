//
//  Entity.h
//  tennis
//
//  Created by Sudeep Bhandari on 2/26/18.
//  Copyright © 2018 Sudeep Bhandari. All rights reserved.
//

#ifndef Entity_h
#define Entity_h

class Entity{
    
    
private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Model model;
public:
    Entity(char* filename): model(filename){
        
    }
    Entity(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,char* filename): model(filename) {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
    }
    
    void setPosition(glm::vec3 pos){
        position = pos;
    }
    void setRotation(glm::vec3 rot){
        rotation = rot;
    }
    void setScale(glm::vec3 sca){
        scale = sca;
    }
    
    glm::vec3 getPosition(){
        return position;
    }
    glm::vec3 getRotation(){
        return rotation;
    }
    glm::vec3 getScale(){
        return scale;
    }
    
    void move(glm::vec3 delta) {
        this->position += delta;
    }
    
    void draw(Shader& shader){
        model.Draw(shader);
    }
    
};


#endif /* Entity_h */
