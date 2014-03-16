function GRN = initGRNForWT()
%   This function initializes a GRN for the Wind Turbine cells
%   This function creates the data structure of the GRN
%   It then initializes the GRN to its default values

%   Based on our meeting, 8 input , 5 output, and the rest regulatory



%   This script defines the internal encoding of the regulatory Networks
%   and its internal dynamics with an identifier, enhancer,
%   inhibitor and type

%   Use a struct to define the internal data structure of a protein

GRN  = struct();


%   --------- DECLARATION OF PROTEIN CONSTANTS --------------   %
%   Amount of protein to evaluate and precision of GRN (p)
GRN.MAX = 64;
GRN.P = 63;

%   Input – Concentration from environment, regulates others but not regulated
%   Output – Concentration used as network output, is regulates but not regulate others
%   Regulatory – Internal protein regulates and is regulated.
%   Values in vectors are randomly generated at concept of GRN, one for
%   each protein in the GRN

%   Type definition: 0 - INPUT, 1 - OUTPUT AND 2 - REGULATORY
%GRN.type = randi([0, 2], 1, GRN.MAX); %% CHANGE THIS LINE.....WELL USE FOR LOOPS OR VECTORIZATION.
%   Preallocate Space for the GRN Proteins
GRN.type = zeros(1, GRN.MAX);

%   Specify the protein types
GRN.type(:, 1:8) = 0;
GRN.type(:,9:13) = 1;
GRN.type(:,14:end) = 2;



%   -------- END OF PROTEIN CONSTANT DECLARATIONS -----------   %
%   Integer Identifier vector
GRN.id = randi([0, GRN.P], 1, GRN.MAX);

%  Integer Enhancer vector
GRN.idEnh = randi([0, GRN.P], 1, GRN.MAX);

%  Integer Inhibitor vector
GRN.idInh = randi([0, GRN.P], 1, GRN.MAX);

%   Creating the concentration vector of type double, for the proteins
%   during initialization, all proteins are initialized to 1/N where N =
%   MAX
GRN.Conc = (ones([1, GRN.MAX]) .* 1.0/GRN.MAX);

%   Defining beta and delta random values between 0.5 and 2
%   Two constants that speed up the reaction of the regulatory network
%   Random integers between 1 and 4 then divide by 2..
GRN.beta = randi([1, 4])/2;
GRN.delta = randi([1, 4])/2;


%   Coding the matching factors for enhancer and inhibitor
%   the resulting values will be put in a pre-allocated MAX-by-MAX matrix
%   Therefore, individual values for the protecin can be directly accessed.

%GRN.matEnhFac = zeros(GRN.MAX); %   preallocation
%GRN.matInhFac = zeros(GRN.MAX);

%   loop through vectorizing
for i = 1 : GRN.MAX
    for j = 1 : GRN.MAX
        GRN.matEnhFac(i,j) = GRN.P - abs(GRN.id(i) - GRN.idEnh(j));
        GRN.matInhFac(i,j) = GRN.P - abs(GRN.id(i) - GRN.idInh(j));
    end
end

%   Calculating the maximum enhancing and inhibiting factors
GRN.matInhmax = max(max(GRN.matInhFac));
GRN.matEnhmax = max(max(GRN.matEnhFac));

% %   What a complete GRN looks like
% fprintf(' \n --- INTERNAL REPRESENTATION OF A PROTEIN --- \n\n')
% GRN %   Display
%
% % %   Initialization of the concentration matric
%  fprintf('\n-- Enhancing Matching Factor -- \n\n');
%  disp(GRN.matEnhFac);
% %
% % fprintf('\n-- Max Enhancing Matching Factor -- \n\n');
% % disp(GRN.matEnhmax);
% %
%  fprintf('\n-- Inhibiting Matching Factor -- \n\n');
%  disp(GRN.matInhFac);
% %
% % fprintf('\n-- Max Enhancing Matching Factor -- \n\n');
% % disp(GRN.matInhmax);


% %   Saving the GRN to a .mat extension
% fileName = 'GRN4Turbines.mat';
% save(fileName,'GRN');
% %clear GRN;

end



