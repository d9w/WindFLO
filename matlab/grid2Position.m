function [tpositions] = grid2Position( DevGrid )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
[r c] = size(DevGrid);
tpositions = [];

%   This displays the model and all that are alive
%   I am taking into consideration, the (0,0) to match Kalyan's function.
for i  = 1 : r;
    for j = 1 : c;
        %   If not alive, pass else do random decision
        if (DevGrid{i,j}.Alive == 1)
            %   Collect i, and j to a point, but first multiply by 500
            t = i * 310;
            v = j * 310;
            temp = [t v];
            tpositions(end+1, :) = temp;
            %tpositions = [t v]
            %fprintf('i is %d and j is %d \n', t, v);
            
        end
    end
end

end

