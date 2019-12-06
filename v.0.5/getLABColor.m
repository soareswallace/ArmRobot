
function labMean = getLABColor(cam, pixelArea)

%
% function labMean = getLABColor(imagem, pixelArea)
%
% Essa fun��o � usada para treinamento no modelo de detec��o de cor.
% ARGUMENTO: 
% vari�vel cam e pixelArea, ao redor do pixel, a ser considerada.
% ALGORITMO:
% Trabalhando no dom�nio HUE, extrai cor. Mais cliques, mais precis�o.
% RETORNO:
% labMean: Vetor 3x3: 1� linha: HSV m�dio, 2�, desvio padr�o, 3�, X,Y, �rea
%
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Hywre Cesar - Novembro 2018
% www.fb.com.br/hycesar
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
imagem = snapshot(cam);         %pegar a imagem
raizArea = sqrt(pixelArea);     %lado do quadrado.
labMean = [];                   %criar vari�vel de retorno.
clf;                            %criar tela que receber� imagem.
warning off;                    %desligar avisos.
imshow(imagem);                 %mostrar imagem em tela.
hold on;                        %manter plots sobrepostos
vLab = rgb2lab(imagem);         %converter para dom�nio HSV
vLab2 = zeros(1,3);
selectedPixels = 0;             %numero de Pixels selecionados
posicao = zeros(1:2);           %guarda posi��es de cada ponto
%%
%loop capturar� pixels clicados e adjac�ncias 5x5 para reduzir ru�do.
while true
    [X,Y,BUTTON] = ginput(1);   %recebe posi��o XY e posi��o bot�o do mouse
    if BUTTON == 3              %avaliar sa�da do loop
        imagem = snapshot(cam);     %pegar uma nova imagem
        imshow(imagem);             %mostrar imagem em tela.
        vLab = rgb2lab(imagem);     %converter para dom�nio HSV
        [X,Y,BUTTON] = ginput(1);   %recebe posi��o XY 
        if BUTTON == 3              %avaliar sa�da do loop
            break;                  %for�ar interrup��o do loop
        end
    end
    selectedPixels = selectedPixels + 1; %atualiza quantidade
    posicao(selectedPixels, :) = [X, Y]; %guarda casa posicao capturada
    
    %recebe hsv localizado
    recorte = vLab(Y-(raizArea-1)/2:Y+(raizArea-1)/2,X-(raizArea-1)/2:X+(raizArea-1)/2,:);
    
    %Matriz com zeros
    temp = zeros(3,1);
    for i = 1 : raizArea        %percorrer a imagem na linha
        for j = 1 : raizArea    %percorrer a imagem na coluna
            temp(1) = temp(1) + recorte(i,j,1); %somar cada canal
            temp(2) = temp(2) + recorte(i,j,2);
            temp(3) = temp(3) + recorte(i,j,3);
        end
    end
    temp = temp / pixelArea; %tirar a m�dia de cada canal pela �rea
    vLab2(selectedPixels,:) = temp;%cria primeira linha da matriz com a media de cada canal
    
    %adicionar um quadrado representando �rea processada da figura
    line([X-(raizArea-1)/2 X+(raizArea-1)/2] , [Y-(raizArea-1)/2 Y-(raizArea-1)/2]);
    line([X+(raizArea-1)/2 X+(raizArea-1)/2] , [Y-(raizArea-1)/2 Y+(raizArea-1)/2]);
    line([X+(raizArea-1)/2 X-(raizArea-1)/2] , [Y+(raizArea-1)/2 Y+(raizArea-1)/2]);
    line([X-(raizArea-1)/2 X-(raizArea-1)/2] , [Y+(raizArea-1)/2 Y-(raizArea-1)/2]);
end
%%
labMean(1,:) = median(vLab2, 1);%media de cada canal, achatando em 1 linha
labMean(2,:) = std(vLab2, 0, 1);     %desvio padrao de cada canal
if selectedPixels < 3
    labMean(3,1) = mean(posicao(:,1)); %m�dia da posi��o X
    labMean(3,2) = mean(posicao(:,2)); %m�dia da posi��o Y
    labMean(3,3) = pixelArea*selectedPixels; %area de dois pontos � nula, logo usar area dos pixels
else
    polig = polyshape(posicao); %cria um pol�gono com os pontos clicados
    plot(polig);                %mostrar poligono em tela

    %centroide do pol�gono formado pelos pontos
    [x,y] = centroid(polig);    %posicao Centr�ide
    labMean(3,1) = x;           %posi��o X
    labMean(3,2) = y;           %posi��o Y
    labMean(3,3) = area(polig); %�rea formadas por esse pol�gono
end
%%
plot(labMean(3,1),labMean(3,2),'r*');%plotar centro do pol�gono
pause(1);                   %dois segundo para verificar posicao
%}
%%
hold off;                   %permite sobreposi��o do plot
warning on;                 %ativa warnings
%close all;                 %fechar janela: Abrir/Fechar -> (perform--)
end