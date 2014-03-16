%This function will give the power for a given velocity. This is a
%standardized power curve given by the A. Kusiak, the function is designed
%such that multiple power curves can be added in future
%model takes the value for which power curve model should one use, if 1 is
%put it will work for the kusiak model. 
function [P]=PowOutput(v,model,vCin,vCout,vRated,PRated)
model=1;
load FarmParameters
if model==1
    if v<vCin 
        P=0;
    elseif v>=vCin && v<=vRated 
        P=lambda*v+eta;
    elseif vCout>v && v>vRated 
        P=PRated;
    else P=0;
    end
end

    