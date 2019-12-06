function [cam1, cam2] = connectStereoCams()
    %
    % function [cam1, cam2] = connectStereoCams()
    %
    % Essa fun��o � usada para, verificar conex�o com as c�meras e retorn�-las
    % em caso de sucesso.
    % ARGUMENTO: 
    % Nenhum.
    % ALGORITMO:
    % Testa conex�o e mostra um preview.
    % RETORNO:
    % cam1 e cam2 para poss�vel calibra��o.
    %
    % %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % Hywre Cesar - Novembro 2018
    % www.fb.com.br/hycesar
    % %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%
    %verifique se as cameras conectadas s�o as certas
    webcamlist
    
    %Foram colocadas juntas por algum motivo, 
    %j� que antes dava timeout
    cam1 = webcam(2);
    %Melhor definir resolu��o 4:3 que � mais 'alta' que o normal.
    %Assim d� pra ver a altura do bra�o melhor.
    cam1.Resolution='1280x960';
    preview(cam1);
    pause(5);
    closePreview(cam1);
    
    %Preferiu abrir a camera e fechar 
    %e depois abrir a segunda, na abordagem anterior
    %dava timeout, talvez por que dava deadlock.
    %N�o alterar
    cam2 = webcam(3);
    cam2.Resolution='1280x960';
    preview(cam2);
    pause(5);
    closePreview(cam2);
    if isempty(cam1) || isempty(cam2)
        error("Verifique sua c�meras!");
    end
    end