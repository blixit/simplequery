#include <iostream>
#include "SQUtils.h"

namespace SQ{   
  
        int PATCH::split(std::vector<std::string>& vecteur, std::string chaine, char separateur)
        {
          vecteur.clear();

          std::string::size_type stTemp = chaine.find(separateur);

          while(stTemp != std::string::npos)
          {
            vecteur.push_back(chaine.substr(0, stTemp));
            chaine = chaine.substr(stTemp + 1);
            stTemp = chaine.find(separateur);
          }

          vecteur.push_back(chaine);

          return vecteur.size();
        } 

}