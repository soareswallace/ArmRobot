function [position] = positionSet(s, goalPos)

    %criar uma comunica��o Serial
    %s = serial('COM3', 'BaudRate', 9600);
    %fopen(s);
    %informa��es da porta na tela
    %get(s);
    % opera��o que cinde informa��o em dois bytes
    low = mod (goalPos, 256);
    high = floor(goalPos/256);

    % ordem deve ser mantida, pois h� uma 'fila' no Ardu�no.
    % lendo posicao, argumento: qnt de bytes a serem lidos.
    % sempre na ordem de low, depois high.
    fwrite(s, low);
    fwrite(s, high);
    %Eu acredito que deva ter um tempo de escrita para o arduino, fechando a porta.
    %� uma Suposi��o: Faltam MAIS testes.
    %fclose(s);
    %fopen(s);
    %receber a posi��o em ordem, sempre.
    %position = fread(s, 1);
    %position = position + fread(s, 1)*256;
    %nada mais vai ser feito at� ent�o.
    %fclose(s);
end