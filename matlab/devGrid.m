function  grid = devGrid( M, N, GRN )
    %   This function creates a developmental model for the wind turbine
    
    %   INPUT: the input is the number of cells we want to evaluate
    %   M - Number of rows and N - Number of columns
    
    %   OUTPUT: It outputs a model of grid with all structures of GRN in
    %   each cell. Now it depends on the addition of cell at a particular
    %   that determines what the output would be
    %   PARAMETER DEFINITION:
    %               Alive = Is there a cell in that grid location?
    %                       Values - Boolean : [0 (Not Alive), 1(Alive)],
    %                       defaults to Not Alive
    %               Direction = What is the direction of the division plan
    %               it is going to be represented as the 8 directions - N,
    %               NE, E, SE, S, SW, W, and NW.
    
    %   Based on the noOfGrid, create a cell array of the amount of cells
    %   in the grid. It produces a noOfGrid - by - noOfGrid cell array
    grid = cell(M,N);
    
    %   Initialize all cells with Alive being  defaulted to boolean(0) and 
    %   direction defaulted to North 
    cellValue = struct();
    cellValue.Alive = boolean(0);
    cellValue.Direction = 'N';
    cellValue.GRN = GRN;
    
    %   Vectorizing the for loop to initialize all cells to the default
    %   cell values
    for i = 1 : M
        for j = 1 : N
            grid{i,j} = cellValue;
        end 
    end
    
    %   View grid
    %disp(grid)

end

