char* toCharFunction(String StingToConvert)
{
  
  static char CharResp[40];
    
  if(StingToConvert){
      StingToConvert.toCharArray(CharResp, StingToConvert.length()+1);
      return CharResp;
  }else{ 
        return nullptr;
      }
}
