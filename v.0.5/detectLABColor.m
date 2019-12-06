function stat = detectLABColor(cam, objLAB)

%
% function hsvAll = detectLABColor(imagem)
%
% Essa fun��o � usada para procurar, quando modelo j� treinado.
% ARGUMENTO: 
% vari�vel cam e objLAB, par�metros da cor, desvioP e posi��o/Area.
% ALGORITMO:
% Trabalhando no dom�nio HUE, extrai cor. Mais cliques, mais precis�o.
% RETORNO:
% hsvAll: Vetor 3x3: 1� linha: LAB m�dio, 2�, desvio padr�o, 3�, posi��o.
%
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Hywre Cesar - Novembro 2018
% www.fb.com.br/hycesar
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
imagem = snapshot(cam);     %pegar a imagem

vLAB = rgb2lab(imagem);     %Converter rgb para Lab

%Encontrar diferen�a entre Hue da imagem a ser detectada e ao ser procurado
difA = abs(vLAB(:,:,2) - objLAB(1,2));
difB = abs(vLAB(:,:,3) - objLAB(1,3));

[M,N,~] = size(imagem);     %Altura, Largura e Profunidade da Imagem

%declarar a vari�vel antes melhora performance.
A = zeros(M,N);
B = zeros(M,N);

%guarda em vari�vel a toler�ncia de cor
TolA = objLAB(2,2);         %um desvio captura 67% das ocorr�ncias
TolB = objLAB(2,3);         
if(TolA < 10 || TolB < 10)%tolerancia baixa atrapalha
    TolA = 20;             %esse valor fixo foi testado.
    TolB = 20;
elseif(TolA > 30 || TolB > 30)
    TolA = 20;             %esse valor fixo foi testado.
    TolB = 20;
end

%evidenciar posi��o onde candidata a cor est� dentro da toler�ncia.
A(difA < TolA) = 1;
B(difB < TolB) = 1;

%remover pequenos objetos brancos, abaixo do limite na dete��o
%pixelAreaLimit = round(abs(objLAB(3,3)));
pixelAreaLimit = 20;
%melhor remover
%{
if pixelAreaLimit < 100
    pixelAreaLimit = 100;
end
%}
A = bwareaopen(A, pixelAreaLimit);
B = bwareaopen(B, pixelAreaLimit);

%remover buracos pretos em qualquer �rea circundada por branco
if abs(objLAB(1,2)) > 5 || abs(objLAB(1,3)) > 5
    imgMerged = imfill(A & B, 8, 'holes'); %ideal para cores.
else
    imgMerged = imfill(A & B, 8);%ideal para preto.
end

%reduziu ru�do juntando por��es coerentes das imagens: eros�o e dilata��o
structShape = strel('disk', 10);      %5 pixels de eros�o/dilata��o
img = imclose(imgMerged, structShape); %

%Mais filtro, talvez desnecess�rio
%imgF = imfill(img, 'holes');
%%
% plotar: Comentar no fim
%imshow(img); title('Resultado');
%}
%%
% procurar objetos: noholes -> simples holes ->mapeia os objetos internos
% 3� Parametro: qntRegioes, 4�, dependencia
[posFronteira, imgEtiquetas, ~, ~] = bwboundaries(img,'noholes');

%estat�stica das posi��es
stat = regionprops(imgEtiquetas, 'Area', 'BoundingBox', 'Centroid', ...
    'EquivDiameter','Perimeter', 'FilledArea');
%%
% mostrar a matriz de etiquetas em cada limite: Comentar no fim!
%imshow(label2rgb(imgEtiquetas, @jet, [.5 .5 .5])) %jet:colormap, []:backgnd
imshow(imagem);
hold on
for k = 1:length(posFronteira)%qntRegioes
  boundary = posFronteira{k};
  plot(boundary(:,2), boundary(:,1), 'w', 'LineWidth', 2)%real�ar bordas
end
%}

%limite para se considerar um "c�rculo" e retornar 1� encontrado.
limiteCircunferencia = 0.10;
% loop sobre cada fronteira para construir imagem, Comentar no fim.
for k = 1:length(posFronteira)%qntRegioes
    %pega a posic�o do blob
    boundary = posFronteira{k};
    % computar o grau de circunfer�ncia do objeto
    metric = 4*pi*stat(k).Area/stat(k).Perimeter^2;
    %%
    % mostrar resultados, Comentar no fim.
    metric_string = sprintf('%2.2f',metric);
    % marcar os objetos que satisfazem o m�nimo de Circunfer�ncia.
    if metric > limiteCircunferencia && metric < 15 * limiteCircunferencia
        centroid = stat(k).Centroid;
        plot(centroid(1), centroid(2), 'r*', 'MarkerSize', 8, 'LineWidth', 1);
        %%
        %Formata��o
        text(boundary(1,2) - 30 , boundary(1,1) + 30 , ...
            strcat('X:', num2str(round(centroid(1))),...
            'Y:', num2str(round(centroid(2)))),...
            'Color','r', 'FontSize',8,'FontWeight','bold');
        %parar procura pelo primeiro circulo achado.
            break;
    else
        %Formata��o
        text(boundary(1,2) - 30 , boundary(1,1) + 30 ,...
            metric_string, 'Color','b', 'FontSize',8);
    end
    %}
end
%%    
%{
%outra forma de procurar, mais eficiente que bwlabel, noholes: d� mesmo
%resultado, holes: n�o.
[componentConectado] = bwconncomp(img);

%determinar o maior objeto
numPixels = cellfun(@numel, componentConectado.PixelIdxList);
[maiorObjeto, idX] = max(numPixels);

%transform�-lo?
%BW(CC.PixelIdxList{idX}) = 0;
%}
%%
%retorna apenas k de interesse
stat = stat(k);
hold off;                   %permite sobreposi��o do plot
warning on;                 %ativa warnings
end