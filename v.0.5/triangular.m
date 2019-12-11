function vetorPos = triangular(stat1, stat2, stereoParams)

%
% function stat = minhaTriangulacao(stat1, stat2)
%
% Essa funcao recebe dois stats e tenta determinar a posi��o.
% ARGUMENTO: 
% duas estruturas stat com a posicao, area, centroid,... de cada blob 
% detectado em cada camera.
% ALGORITMO:
% Verifica a validade dos dois stats e, se forem coerentes, triangula.
% RETORNO:
% vetorPos: Vetor 3x1: Posi��o X, Y, Z.
%
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Hywre Cesar - Novembro 2018
% www.fb.com.br/hycesar
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%4%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
vetorPos = zeros(1,3);
% As duas c�meras detectaram o ponto?
if ~isempty(stat1) && ~isempty(stat2) && ~isempty(stereoParams)
    tol = max(stat1.EquivDiameter, stat2.EquivDiameter);
    % � uma dist�ncia v�lida?
    if stat1.Centroid - stat2.Centroid < tol*4
        %triangula��o: descobrir a distancia com a calibra��o anterior.
            vetorPos = triangulate(stat1.Centroid, stat2.Centroid, stereoParams);
    else
        warning("N�o foi possivel triangular com seguran�a: " + ...
        "Possivelmente n�o s�o o mesmo ponto! Verifique seus par�metros");
    end
else
    warning("Uma das c�meras n�o retornou um ponto v�lido, neste momento!");
end
end