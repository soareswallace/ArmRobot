function [params] = quickStereoCalibration(cam1, cam2)
    %
    % function [params] = quickStereoCalibration(cam1, cam2)
    %
    % Essa fun��o � usada para rapidamente calibrar quando na primeira vez e 
    % quando necess�rio.
    % ARGUMENTO: 
    % cam1 e cam2 a serem calibradas.
    % ALGORITMO:
    % Utiliza s�rie de fun��es de calibra��o.
    % RETORNO:
    % params s�o par�metros da camera para utilizar triangula��o, depois.
    %
    % %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % Hywre Cesar - Novembro 2018
    % www.fb.com.br/hycesar
    % %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%
    %locais onde iram ser salvas as imagens
    %Esses locais DEVEM existir, antes de executar o c�digo.
    path1 = strcat(pwd, '\cameraStereo1');
    path2 = strcat(pwd, '\cameraStereo2');
    
    if ~exist(path1)
        mkdir(path1)
    end
    
    if ~exist(path2)
        mkdir(path2)
    end
    %%
    %la�o
    n = 0;
    while n < 50            %~30 � qnt recomendada de imagens para calibra��o
        img1 = snapshot(cam1);
        img2 = snapshot(cam2);
        n
        imshowpair(img1, img2, 'montage');
        
        %colocar nas pastas 
        filename1 = [strcat(path1, '\'), int2str(n), '.png'];
        filename2 = [strcat(path2, '\'), int2str(n), '.png'];
        
    
        
        %Guardar as fotos
        imwrite(img1, fullfile(filename1));
        imwrite(img2, fullfile(filename2));
        
        %Espera o usu�rio se posicionar para a pr�xima foto
        %Tempo baixo causa borrado nas fotos
        pause(4);
    
        %Controla la�o e nome do arquivo
        n = n + 1;
    end
    %%
    %pasta com imagens
    leftImages = imageDatastore(path1);
    rightImages = imageDatastore(path2);    
    
    %detectar tabuleiro
    [imagePoints,boardSize] = detectCheckerboardPoints(leftImages.Files, rightImages.Files);
    
    %Especifica tamanho da c�lula em mm
    squareSize = 15;                    %importante: depende do tabuleiro (mm)
    worldPoints = generateCheckerboardPoints(boardSize,squareSize);
    
    %Pegar o tamanho das imagens e calibrar
    I = readimage(leftImages,1); 
    imageSize = [size(I,1),size(I,2)];
    params = estimateCameraParameters(imagePoints, worldPoints, 'ImageSize',imageSize);
    %%
    %Mostrar precis�o da calibra��o
    showReprojectionErrors(params);
    
    %mostrar distribui��o da c�mera no espa�o
    figure;
    showExtrinsics(params);
    %}
    end