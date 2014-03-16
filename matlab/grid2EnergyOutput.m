function [TSpE2 EnergyCapturedWithDecisionRandom] = grid2EnergyOutput(DevGrid)
%   This function takes in a developmental grid, and calculates how much
%   the total energy from the living cells would produce.

%   The input is a devModel, it checks to see which cells are alive, then
%   collects their cordinates. After that, it multiplies these cordinates
%   by 500 and passes the output to EnergyOutput.m, which helps to
%   calculate the output energy using flag=2.

%   Collect tpositions from the developmental grid into a vector and pass it to
%   EnergyOutput.m

%   Size of the grid
flag = 2;

tpositions = grid2Position(DevGrid);

%tpositions

%   Find out how many elements are there in captured tpositions
[r c] = size(tpositions);
if (r>0)
    if(r == 1) % Then there is just one surviving turbine and will have a lossless capture, so we set
        %   tpositions to [0,0] and flag to 1 according to kalyans function.
        tpositionLossless = [0 0];
        flagLossless = 1;
        [TSpE EnergyCapturedWithDecisionRandom] = EnergyOutput(tpositionLossless,flagLossless);
    else if (r > 1)
            [TSpE EnergyCapturedWithDecisionRandom] = EnergyOutput(tpositions,flag);
        end
    end
    [r c] = size(TSpE);
    TSpE2=[TSpE(r-1,:);TSpE(r,:);TSpE(1:r-2,:)];
else
    TSpE2=zeros([1,8]);
    EnergyCapturedWithDecisionRandom=0;
end

%TSpE2


