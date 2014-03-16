function updatedDevGrid = stepForward(devGrid,simStep)
%   This function takes in a devGrid with or without cells and returns and
%   updated Developmental Grid.

%   Based on the Grid, it checks each cell and determines which decision to
%   take for that cell.
%   Right now that decision is just a random set of decisions.

%   Get the rows and column values for the devGrid
[r c] = size(devGrid);
updatedDevGrid = devGrid;
devGridToProcess = devGrid;
%   For each cell c in the grid...
for i  = 1 : r
    for j = 1 : c
        %   If not alive, pass else do random decision
        if (devGridToProcess{i,j}.Alive == 0)
            continue;
        else
            liveCell = devGridToProcess{i,j};
            %response = decisionRandom(updatedDevGrid, liveCell, i, j);
            response = decisionGRN(updatedDevGrid, liveCell, i, j);
            
            %   if -1 is returned its an invalid response so return
            if (response == -1)
                continue;
            else
                updatedDevGrid = applyAction(updatedDevGrid, i, j, response, simStep);
            end
        end
    end
end
end

