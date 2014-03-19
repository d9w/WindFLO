classdef WindFarmLayoutEvaluator
    % internal variables, no user access
    properties (Hidden)
        dcos = [];
        esin = [];
        v1large = [];
        v2large = [];
        Plarge = [];
    end
    % read-access variables, simply call WFLE.var_name
    properties (SetAccess = private)
        nEvals = 0;             % number of evaluations run so far
        wfRatio = 0;            % current farm wake free ratio
        EnergyOutputs = [];     % Energy per turbine per direction, n*24
        TurbineFitnesses = [];  % Wake free ratio for each turbine, n*2
        ws;                     % Wind scenario contains farm parameters
    end
    methods            
        % Constructor
        % Initializes the evaluator with a wind scenario
        % This method doesn't increase the number of evaluations counter.
        % @param scenario
        function WFLE = WindFarmLayoutEvaluator(WindScenario)
            WFLE.ws = initialize(WindScenario);
        end
        
        % Evaluate
        % Evaluates a given layout and returns its wake free ratio
        % This method increases the number of evaluations counter.
        % @param layout The layout to evaluate, n*2 matrix of [x[], y[]]
        % sets the wake free ratio of the layout, double between 0, 1,
        % or -1 is the layout is invalid
        function WFLE = evaluate(WFLE, Layout)
            if WFLE.CheckConstraint(Layout)
                % set the convenience matrices
                WFLE = WFLE.SettingsMatrices(Layout);
                [TotalEnergy,TSpE] = WFLE.WindResourcePerTurbine(Layout);
                WFLE.wfRatio = TotalEnergy./...
                    (size(Layout,1)*WFLE.ws.energy);
                WFLE.EnergyOutputs = TSpE';
                WFLE.TurbineFitnesses = sum(WFLE.EnergyOutputs,1)./...
                    WFLE.ws.energy;
            else
                WFLE.wfRatio = -1;
                WFLE.EnergyOutputs = zeros(size(Layout,1),...
                    size(WFLE.ws.thetas,1));
                WFLE.TurbineFirnesses = -1.*ones(size(Layout,1),1);
            end
            WFLE.nEvals = WFLE.nEvals + 1;
        end
    end
    % Private methods
    methods (Access=private, Hidden)
        
        % Check the security constraint
        function check = CheckConstraint(WFLE, tpositions)
            R=WFLE.ws.R;
            [D]=Mdist(tpositions);
            tempvec=65*R^2*ones(1,length(tpositions(:,1)));
            RMat=diag(tempvec);
            D=D+RMat;
            %Diagnols are fixed. 
            check=isempty(find(D<=8*R,1));
        end
        
        % Convenience matrices
        function WFLE = SettingsMatrices(WFLE, tpositions)
            fac=pi/180;
            thetas=WFLE.ws.thetas;
            coses=cos(mean(thetas.*fac,2));
            sins=sin(mean(thetas.*fac,2));
            largecos=repmat(coses,1,length(tpositions(:,1)));
            largesin=repmat(sins,1,length(tpositions(:,1)));
            %change the next statement to have transpose 
            bcoses=reshape(largecos',numel(largecos),1);
            csins=reshape(largesin',numel(largesin),1);
            WFLE.dcos=repmat(bcoses,1,length(tpositions(:,1)));
            WFLE.esin=repmat(csins,1,length(tpositions(:,1)));
            %power model
            vints=3.5:0.5:WFLE.ws.vRated;
            v1=vints(2:1:end);
            v2=vints(1:1:length(vints)-1);
            %v=(v1+v2)/2;
            WFLE.v1large=repmat(v1',length(thetas(:,1)),...
                length(tpositions(:,1)));
            WFLE.v2large=repmat(v2',length(thetas(:,1)),...
                length(tpositions(:,1)));
            for ghh=2:1:length(vints)
                v=(vints(ghh)+vints(ghh-1))/2;
                if v<WFLE.ws.vCin 
                    p=0;
                elseif v>=WFLE.ws.vCin && v<=WFLE.ws.vRated 
                    p=WFLE.ws.lambda*v+WFLE.ws.eta;
                elseif WFLE.ws.vCout>v && v>WFLE.ws.vRated 
                    p=WFLE.ws.PRated;
                else p=0;
                end
                P(ghh-1)=p;
            end
            WFLE.Plarge=repmat(P',length(thetas(:,1)),...
                length(tpositions(:,1)));
        end
        
        % wind energy per turbine
        function [TotalEnergy, TSpE] = ...
                WindResourcePerTurbine(WFLE, tpositions)

            xx=tpositions(:,1)';
            yy=tpositions(:,2)';
            [xx1,xx2]=meshgrid(xx);
            [yy1,yy2]=meshgrid(yy);
            const1=WFLE.ws.R/WFLE.ws.k;
            xd=(xx1-xx2)';
            yd=(yy1-yy2)';
            %changed it to 1, added transpose to this operation 
            xlarge=repmat(xd,[length(WFLE.ws.thetas(:,1)) 1]);
            ylarge=repmat(yd,[length(WFLE.ws.thetas(:,1)) 1]);

            %now create the beta function 
            alpha=atan(WFLE.ws.k);
            beta=acos((xlarge.*WFLE.dcos+ylarge.*WFLE.esin +const1)./...
                sqrt((xlarge+const1*WFLE.dcos).^2+...
                (ylarge+const1*WFLE.esin).^2)); 

            %check for the presence of the wake
            wakepresence=beta<=alpha;
            wakepresence(find(beta<0.000001))=0;

            %calculate the projected distance 
            pjdist=abs(xlarge.*WFLE.dcos+ylarge.*WFLE.esin);
            % calculate the wake effects 
            a=1-sqrt(1-WFLE.ws.CT);
            b=WFLE.ws.k/WFLE.ws.R;
            VelDef=a./((1+b.*pjdist).^2);

            %add up the wake based on its presence 
            wk=wakepresence.*VelDef;
            TotalVdef=sqrt(sum((wk.^2),2));

            % change the wind resource 
            temp1=repmat(WFLE.ws.cs,[length(tpositions(:,1)) 1]);
            crep=reshape(temp1,numel(temp1),1);
            crep=crep.*(1-TotalVdef);
            lp=1;
            vints=3.5:0.5:WFLE.ws.vRated;
            wholecs=[];
            wholeks=[];
            wholeoms=[];
            wholethets=[];
            smallcs=[];
            smallks=[];
            smalloms=[];
            smallthets=[];

            for i=1:1:length(tpositions(:,1))
                cs(i,:)=crep(lp:length(tpositions(:,1)):end)';
                kk=crep(lp:length(tpositions(:,1)):end)';
                kss(i,:)=WFLE.ws.ks;
                kkss=WFLE.ws.ks;
                lp=lp+1;
                thets(i,:)=(WFLE.ws.thetas(:,2)-WFLE.ws.thetas(:,1))';
                omegs(i,:)=WFLE.ws.omegas;       

                largec=repmat(kk',1,length(vints)-1);
                largeks=repmat(kkss',1,length(vints)-1); 
                bc=reshape(largec',prod(size(largec)),1);
                cks=reshape(largeks',prod(size(largeks)),1);

                largeom=repmat(omegs(i,:)',1,length(vints)-1);
                largethets=repmat(thets(i,:)',1,length(vints)-1);
                doms=reshape(largeom',prod(size(largeom)),1);
                ethets=reshape(largethets',prod(size(largethets)),1);

                wholecs=[wholecs bc];
                wholeks=[wholeks cks];
                wholeoms=[wholeoms doms];
                wholethets=[wholethets ethets];
                smallcs=[smallcs cs(i,:)'];
                smallks=[smallks kss(i,:)'];
                smalloms=[smalloms WFLE.ws.omegas'];
                smallthets=[smallthets thets(i,:)'];
            end

            prV=wblcdf(WFLE.v1large,wholecs,wholeks)...
                -wblcdf(WFLE.v2large,wholecs,wholeks);

            nTurbines=size(tpositions,1);
            pp=wholeoms.*wholethets.*prV.*WFLE.Plarge;
            PRlarge=repmat(WFLE.ws.PRated,length(WFLE.ws.thetas(:,1)),...
                nTurbines);
            VRlarge=repmat(WFLE.ws.vRated,length(WFLE.ws.thetas(:,1)),...
                nTurbines);
            wbPrated=1-wblcdf(VRlarge,smallcs,smallks);
            prat=wbPrated.*PRlarge.*smalloms.*smallthets;
            r1=sum(sum(pp));
            r2=sum(sum(prat));
            TotalEnergy=r1+r2;

            h=1;
            for hj=1:21:length(pp(:,1))
                temp=pp(hj:hj+20,:);
                TSpE(h,:)=sum(temp);
                h=h+1;
            end
            TSpE=TSpE+prat;
        end
    end 
end