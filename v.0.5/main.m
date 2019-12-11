arquivo = fopen('dados.txt','w');

%Codigo de comunicacao com as cameras
 [cam1, cam2] = connectStereoCams;
 load('stereoParams.mat');

%Codigo de comunicacao com o robo
 s = serial('/dev/tty.usbserial-AI069KTW', 'BaudRate', 9600);

 fopen(s);

qntd = 1000;

[pos1,pos2,pos3,pos4] = generate(qntd);

 fwrite(s, 'S');
 disp(fscanf(s));
 
%Comando para ir � posicao para pegar a cor
fwrite(s,'i','uchar');
pause(5);

%Pegar cor base
% stereoParams = quickStereoCalibration(cam1, cam2);
 LAB = getLABColor(cam1, 81);

a=1;
while(a<=qntd)
    %Parte de envio e recebimento das posicoes para o robo
    fprintf('Dado numero: %d \n',a);
    disp([pos1(a), pos2(a), pos3(a), pos4(a)]);
     writePosition(s, pos1(a));
     writePosition(s, pos2(a));
     writePosition(s, pos3(a));
     writePosition(s, pos4(a));
     disp(fscanf(s));
   
    pause(2);
    
    %Parte de triangulacao de distancia pelas cameras
     stat1 = detectLABColor(cam1, LAB);
     stat2 = detectLABColor(cam2, LAB);
     vetorPos(a, :) = triangular(stat1, stat2, stereoParams); 
     debug('b', vetorPos(a, :));
     debug('w', vetorPos(a, :));
    
    %parte de guardar os dados no arquivo [num motor2 motor3 motor4 motor5 X Y Z]
     fprintf(arquivo,'%d %d %d %d %d %f %f %f \n', a, pos1(a), pos2(a), pos3(a), pos4(a), vetorPos(a, 1), vetorPos(a, 2), vetorPos(a, 3));
%     fprintf(arquivo,'%d %d %d %d %d \n', a, pos1(a), pos2(a), pos3(a), pos4(a));

    pause(2);
    a = a + 1;
end

%Comando para robo ir para posicao home
fwrite(s,'h','uchar');
pause(5);

%Fechar conex�o com serial do robo
 fclose(s);
 delete(s);
 clear s;