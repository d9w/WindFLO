% This function does the double numerical integration for a given turbine
% and comes up with the estimate. Inputs for this function are the weibull
% parameters (modified based on the wake)
function [energy]=EnergyEstimatePerTurbine(c,thetas,omegas,ks,P);
% veloccity intervals between 0-cut-out
model=1;
load FarmParameters

  prV=wblcdf(vints(ghh),ci,ki) -wblcdf(vints(ghh-1),ci,ki);
    powV(kal)=prV*P;
for hk=1:1:length(thetas(:,1))
    tint=thetas(hk,2)-thetas(hk,1);
    w=omegas(hk);
    ci=c(hk);
    ki=ks(hk);
    kal=1;
   
    totalPow=sum(powV)+PRated*(1-wblcdf(vRated,ci,ki));
    
    pwr(hk)=tint*w*totalPow;
end
energy=sum(pwr);
