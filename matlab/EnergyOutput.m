function EnergyCapture=EnergyOutput(tpositions,flag)
 
if flag==1
    tpositions=[ tpositions; 1000000 1000000];
    settings=SettingMatrices(tpositions);
    dcos = settings.dcos;
    esin = settings.esin;
    Plarge = settings.Plarge;
    v1large = settings.v1large;
    v2large = settings.v2large;
    Re_tpos=reshape(tpositions',1,[]);
    EnergyCapture=fitnessWTLayoutnew(Re_tpos,dcos,esin,Plarge,v1large,v2large);
else 
    %tpositions=[ 0 0]
    settings=SettingMatrices(tpositions);
    dcos = settings.dcos;
    esin = settings.esin;
    Plarge = settings.Plarge;
    v1large = settings.v1large;
    v2large = settings.v2large;
    Re_tpos=reshape(tpositions',1,[]);
    EnergyCapture=fitnessWTLayoutnew(tpositions,dcos,esin,Plarge,v1large,v2large);
end 
