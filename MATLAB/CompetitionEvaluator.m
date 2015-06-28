classdef CompetitionEvaluator
    % read-access variables, simply call WFLE.var_name
    properties (SetAccess = private)
        EnergyCost = 0;     % cost of energy, 2015 evaluation function
        nEvals = 0;             % number of evaluations run so far
        wfRatio = 0;            % current farm wake free ratio
        EnergyOutput = 0;       % Energy capture of the field
        EnergyOutputs = [];     % Energy per turbine per direction, n*24
        TurbineFitnesses = [];  % Wake free ratio for each turbine, n*2
        ws;                     % Wind scenario contains farm parameters
        api_token = ''; % user token
        run_token = ''; % run token
    end
    methods            
        % Constructor
        % Initializes the evaluator with a wind scenario
        % This method doesn't increase the number of evaluations counter.
        % @param scenario
        function WFLE = CompetitionEvaluator(WindScenario, varargin)
            % initialize wind scenario
            options = weboptions('Timeout',20);
            scenario = webread(sprintf('http://windflo.com/scenarios/%d',WindScenario),options);
            obsize = size(scenario.obstacles, 1);
            obstacles = zeros(obsize,4);
            for i=1:obsize
                obstacles(i,1) = scenario.obstacles(i).xmin;
                obstacles(i,2) = scenario.obstacles(i).ymin;
                obstacles(i,3) = scenario.obstacles(i).xmax;
                obstacles(i,4) = scenario.obstacles(i).ymax;
            end
            ws.number = WindScenario;
            ws.height = scenario.height;
            ws.width = scenario.width;
            ws.obstacles = obstacles;
            ws.CT=0.8;
            ws.PRated=1500.0;
            ws.R=38.5;
            ws.eta=-500.0;
            ws.k=0.0750;
            ws.lambda=140.86;
            ws.vCin=3.5;
            ws.vCout=20;
            ws.vRated=14;
            WFLE.ws = ws;
            WFLE. api_token = varargin{1};
            if nargin>2
                WFLE.run_token = varargin{2};
            else
                options = weboptions('MediaType', 'application/json','Timeout',20);
                data = struct('api_token',WFLE.api_token);
                results = webwrite('http://windflo.com/runs/', data, options);
                WFLE.run_token = results.token;
            end
        end
        
        % Evaluate
        % Evaluates a given layout and returns its wake free ratio
        % This method increases the number of evaluations counter.
        % @param layout The layout to evaluate, n*2 matrix of [x[], y[]]
        % sets the wake free ratio of the layout, double between 0, 1,
        % or -1 is the layout is invalid
        function WFLE = evaluate(WFLE, Layout)
            options = weboptions('MediaType', 'application/json','Timeout',60);
            data = struct('api_token',WFLE.api_token,'run',WFLE.run_token,'scenario',WFLE.ws.number, 'xs',Layout(:,1), 'ys',Layout(:,2));
            results = webwrite('http://windflo.com/evaluate/', data, options);
            WFLE.EnergyOutput = results.energy_output;
            WFLE.wfRatio = results.wake_free_ratio;
            WFLE.EnergyOutputs = results.energy_outputs;
            WFLE.nEvals = results.evals;
            WFLE.EnergyCost = results.energy_cost;
            WFLE.TurbineFitnesses = results.turbine_fitnesses;
        end

        % Check the security constraint
        function check = CheckConstraint(WFLE, tpositions)
            R=WFLE.ws.R;
            [D]=squareform(pdist(tpositions));
            tempvec=65*R^2*ones(1,length(tpositions(:,1)));
            RMat=diag(tempvec);
            D=D+RMat;
            %Diagnols are fixed. 
            check=isempty(find(D<=8*R,1));
            % check boundaries
            if check
                if any([any(any(tpositions<0)),any(tpositions(:,1)>WFLE.ws.width),any(tpositions(:,2)>WFLE.ws.height)])
                    check=0;
                end
            end
            % check obstacles
            if check
                for i=1:size(tpositions,1)
                    for o=1:size(WFLE.ws.obstacles,1)
                        if all([tpositions(i,:)>WFLE.ws.obstacles(o,1:2) ...
                                tpositions(i,:)<WFLE.ws.obstacles(o,3:4)])
                            check=0;
                            break;
                        end
                    end
                end
            end
        end
    end
    
    % Private methods
    methods (Access=private, Hidden)  

    end 
end
