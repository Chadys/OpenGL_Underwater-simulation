
// http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=15
float getFogFactor(FogParameters params, float fFogCoord)
{
   float fResult = 0.0;
   if(params.iEquation == 0)
      fResult = (params.fEnd-fFogCoord)/(params.fEnd-params.fStart);
   else if(params.iEquation == 1)
      fResult = exp(-params.fDensity*fFogCoord);
   else if(params.iEquation == 2)
      fResult = exp(-pow(params.fDensity*fFogCoord, 2.0));

   fResult = 1.0-clamp(fResult, 0.0, 1.0);

   return fResult;
}