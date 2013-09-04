% Parámetros:
%   csv                Path al archivo CSV con la muestra a graficar.
%   titulo             El título del gráfico.
%   unidad             Unidad de tiempo. Valores posibles:
%    - 'milisegundos'
%    - 'microsegundos'
%   divisor            Expresión que divide las mediciones. Valores posibles:
%    - '1'             Sin división.
%    - 'n'             Divide las mediciones por n.
%    - 'log(n)'        Divide las mediciones por log(n).
%   ajuste             Curva de ajuste. Valores posibles:
%    - 'none'          Sin curva de ajuste.
%    - 'n'
%    - 'log(n)'
%    - 'n*log(n)'
%   coef               Coeficiente de la curva de ajuste.
%   inicio             Medición inicial a partir de la cual graficar.

function graficar(csv,
                  titulo,
                  unidad  = 'milisegundos',
                  divisor = '1',
                  ajuste  = 'none',
                  coef    = 1,
                  inicio  = 1)

data = csvread(csv);
n    = (inicio : rows(data))'; % data(inicio:end, 1);
t_n  = data(inicio:end, 2);

if(strcmp(unidad, 'milisegundos'))
    % no hacer nada
elseif(strcmp(unidad, 'microsegundos'))
    t_n = t_n * 1000;
else
    error('Parámetro "unidad" inválido.');
    exit(-1);
end

if(strcmp(divisor, '1'))
    expr    = t_n;
    leyenda = ['$t_n$'];
elseif(strcmp(divisor, 'n'))
    expr    = t_n ./ n;
    leyenda = ['$t_n / n$'];
elseif(strcmp(divisor, 'log(n)'))
    expr    = t_n ./ log(n);
    leyenda = ['$t_n / log(n)$'];
else
    error('Parámetro "divisor" inválido.');
    exit(-1);
end

figure;
hold on;
plot(n, expr);

if(strcmp(ajuste, 'none'))
    % no hacer nada
elseif(strcmp(ajuste, 'n'))
    expr_ajuste   = coef * n;
    leyenda = [leyenda; '$n$'];
elseif(strcmp(ajuste, 'log(n)'))
    expr_ajuste    = coef * log(n);
    leyenda = [leyenda; '$log(n)$'];
elseif(strcmp(ajuste, 'n*log(n)'))
    expr_ajuste    = coef * n .* log(n);
    leyenda = [leyenda; '$n * log(n)$'];
else
    error('Parámetro "ajuste" inválido.');
    exit(-1);
end

if(exist('expr_ajuste'))
    plot(n, expr_ajuste, 'color', 'red');
end

legend(leyenda);
legend('boxon');
legend('location', 'northwest');

title(titulo);

xlabel('$n$ (tamaño del problema)');
if(strcmp(unidad, 'milisegundos'))
    ylabel('Tiempo de ejecución [$mS$]');
else
    ylabel('Tiempo de ejecución [$\mu S$]');
end

xlim([inicio rows(data)]);

hold off;