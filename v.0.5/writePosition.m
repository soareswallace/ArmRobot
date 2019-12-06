% Josenildo Vicente - jva@cin.ufpe.br 

function [signalPos,pos1,pos2] = writePosition(serialObject, goalPos)

    %   signalPos = 0 -> numero negativo  || signalPos = 1 -> numero positivo/zero
    if(goalPos < 0)
        signalPos = 1;
    else
        signalPos = 0;
    end
    
    % posGoalPos -> posicao em numeros positivos
    posGoalPos = abs(goalPos);

    % pos1 -> sera modulo da posicao por 256
    pos1 = mod(posGoalPos,256);
    % pos2 -> sera a divisao da posicao por 256
    pos2 = floor(posGoalPos/256);
    %depois para juntar novamente sera (pos2 * 256) + pos1

    fwrite(serialObject, signalPos, 'int8');
    fwrite(serialObject, pos1, 'int8');
    fwrite(serialObject, pos2, 'int8');
    
end