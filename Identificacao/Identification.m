load('captura.mat');
T_stop = 100;
tout = 1:.1:100;

%% Obtendo dados do sistema
t = tout';
x = dados(:,1:4)'; %Entrada da planta
y = dados(:,5:7)'; %Resposta da planta
N = length(x); %Número de Amostras
variacaoNumEntradas = [7 10]; %Luiz Felipe
%variacaoNumEntradas = [13 25]; %Wallace
params = zeros(216,6); %Luiz Felipe
erros_abs = zeros(216); %Luiz Felipe
des_erros = zeros(216,3); %Luiz Felipe
erros_mse = zeros(216); %Luiz Felipe
histories = cell(216);
%params = zeros(288); %Wallace
%erros_abs = zeros(288); %Wallace
%des_erros = zeros(288); %Wallace
%erros_mse = zeros(288); %Wallace
count = 1;
for a = 1:length(variacaoNumEntradas)
	%Rede Neural
	variacaoNumNeuronios = [20 80];%Luiz Felipe
	%variacaoNumNeuronios = [3 100 300 500];%Wallace
	for b = 1:length(variacaoNumNeuronios)
		variacaoLR = [0.25 0.5]; %Luiz Felipe
		%variacaoLR = [0.001 0.01];%Wallace
		for c = 1:length(variacaoLR)
			variacaoLR_DEC = [0.1 0.5]; %Luiz Felipe
			%variacaoLR_DEC = [0.25 0.7 0.9];%Wallace
			for d = 1:length(variacaoLR_DEC)
				variacaoMC = [0 0.1 0.2]; %Luiz Felipe
				%variacaoMC = [0.5 0.7 0.9];%Wallace
				for e = 1:length(variacaoMC)
					variacaoEpocas = [2 10 100]; %Luiz Felipe
					%variacaoEpocas = [300 1000];%Wallace
					for f = 1:length(variacaoEpocas)
						numEntradas = variacaoNumEntradas(a); %param (entradas incluindo realimentação) 6 8 16 32
						Atraso = numEntradas - 4; %[2 4 12 28]
						u = zeros(numEntradas,N); %Inicialização do vetor de entrada com zeros
						
						%Contrução do vetor de entrada a partir de um loop
						for i = (Atraso+1):N
							temp = x(:,(i-1));
							for k = 1:Atraso/3
								temp = [temp; y(:,(i-k))];
							end
							u(:,i) = temp;
						end
						
						% Feed-forward Backpropagation network
						numNeuronios = variacaoNumNeuronios(b);%[3 20 80 100 300 500]
						net.name = strcat('rede ',num2str(count));
						net = feedforwardnet(numNeuronios);
						net.trainParam.goal = 1e-10; %Erro Mínimo Desejado
						net.trainParam.epochs = variacaoEpocas(f); %Número Máximo de Épocas
						net.trainParam.lr = variacaoLR(c);
						net.trainParam.lr_dec = variacaoLR_DEC(d);
						net.trainParam.mc = variacaoMC(e);
						net.trainParam.showWindow=0; %Para não mostrar o treinamento.
						params(count,:) = [numEntradas numNeuronios net.trainParam.lr net.trainParam.lr_dec net.trainParam.mc net.trainParam.epochs];
						%Treinamento da Rede Neural
						[net,tr] = train(net,u,y);
						this_history_name = strcat('historico da rede ',num2str(count),'.mat')
						save(this_history_name, 'tr');
						%Resposta da Rede Treinada
						resposta_net = net(u);
					
						erros_abs(count) = mae(net,y,resposta_net); %Erro Médio Absoluto da Rede Neural
						des_erros(count,:) = std(y - resposta_net,0,2); %Desvio padrão do erro
						erros_mse(count) = mse(net,resposta_net,y);

						count = count + 1;
					end
				end
			end
		end
	end
end
save 'params.mat' 'params';
save 'erros_mse.mat' 'erros_mse';
save 'erros_abs.mat' 'erros_abs';
save 'des_erros.mat' 'des_erros';