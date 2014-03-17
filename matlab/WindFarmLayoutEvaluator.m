classdef WindFarmLayoutEvaluator
    %% internal variables, no user access
    properties (Hidden) 
        dcos = [];
        esin = [];
        Plarge = [];
        v1large = [];
        v2large = [];
    end
    %% read-access variables, simply call WFLE.var_name
    properties (SetAccess = private)
        nEvals = 0;             % number of evaluations run so far
        wfRatio = 0;            % current farm wake free ratio
        EneryOutputs = [];      % Energy per turbine per direction, n*24
        TurbineFitnesses = [];  % Wake free ratio for each turbine, n*2
    end
    methods            
        %% Initialize
        % Initializes the evaluator with a wind scenario
        % This method doesn't increase the number of evaluations counter.
        % @param scenario
        function WFLE = WindFarmLayoutEvaluator(WindScenario)
            [WFLE.dcos,WFLE.esin,WFLE.Plarge,WFLE.v1large,WFLE.v2large]...
                =initialize(); % see initialize.m
        end
        
       %% Evaluate
        % Evaluates a given layout and returns its wake free ratio
        % This method increases the number of evaluations counter.
        % @param layout The layout to evaluate, n*2 matrix of [x[], y[]]
        % @return The wake free ratio of the layout, double between 0, 1,
        % or -1 is the layout is invalid
        function wfRatio = evaluate(WFLE, Layout)
            % return wfratio or negative if invalid
            % layout is double[][]
            [TotalEnergy, TsPE] = fitnessWTLayoutnew(Layout,...
                WFLE.dcos, WFLE.esin, WFLE.Plarge, WFLE.V1large, WFLE.v2large);
            WFLE.nEvals = WFLE.nEvals + 1;
            wfRatio = WFLE.wfRatio;
        end
    end
end