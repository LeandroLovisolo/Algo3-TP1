% Recibe como parámetro una estructura con los siguientes campos:
%
%   csv                Path al archivo CSV con la muestra a graficar.
%   titulo             El título del gráfico.
%   unidad (opcional)  Unidad de tiempo. Valores posibles:
%    - 'milisegundos'
%    - 'microsegundos'
%   escala (opcional)  Escala a usar para el eje y. Valores posibles:
%    - 'lineal'
%    - 'logaritmica'
%   divisor (opcional) Expresión que divide las mediciones. Valores posibles:
%    - '1'             Sin división.
%    - 'n'             Divide las mediciones por n.
%    - 'log(n)'        Divide las mediciones por log(n).
%   ajuste (opcional)  Curva de ajuste. Valores posibles:
%    - 'none'          Sin curva de ajuste.
%    - 'n'
%    - 'log(n)'
%    - 'n*log(n)'
%   coef (opcional)    Coeficiente de la curva de ajuste.
%   ylim (opcional)    Valor máximo a plottear en el eje y.
%   inicio (opcional)  Medición inicial a partir de la cual graficar.
%   ticks_x (opcional) Usar las etiquetas en el CSV (primera  columna) para
%                      etiquetar el eje x en vez del tamaño del problema.
%   xlabel (opcional)  Etiqueta para el eje x.

function graficar(s)

% Parámetros por defecto.
if(!isfield(s, 'unidad'));  s.unidad  = 'milisegundos';                end
if(!isfield(s, 'escala'));  s.escala  = 'lineal';                      end
if(!isfield(s, 'divisor')); s.divisor = '1';                           end
if(!isfield(s, 'ajuste'));  s.ajuste  = 'none';                        end
if(!isfield(s, 'coef'));    s.coef    = 1;                             end
if(!isfield(s, 'ylim'));    s.ylim    = 'none';                        end    
if(!isfield(s, 'inicio'));  s.inicio  = 1;                             end
if(!isfield(s, 'ticks_x')); s.ticks_x = false;                         end
if(!isfield(s, 'xlabel'));  s.xlabel  = '$n$ (tama\~no del problema)'; end

% Lee las etiquetas de cada medición y las mediciones
% en label_ y t_n, respectivamente.
[label_n t_n] = textread(s.csv, '%s %f');

% Tamaños de los problemas medidos.
n = (s.inicio : rows(t_n))';

% Descartamos las mediciones por fuera del intervalo que se quiere graficar.
label_n = label_n(s.inicio:end);
t_n     = t_n(s.inicio:end);

if(strcmp(s.unidad, 'milisegundos'))
    % No hacer nada.
elseif(strcmp(s.unidad, 'microsegundos'))
    t_n = t_n * 1000;
else
    error('Parámetro "unidad" inválido.');
    exit(-1);
end

if(strcmp(s.divisor, '1'))
    expr    = t_n;
    leyenda = ['$T(n)$'];
elseif(strcmp(s.divisor, 'n'))
    expr    = t_n ./ n;
    leyenda = ['$T(n) / n$'];
elseif(strcmp(s.divisor, 'log(n)'))
    expr    = t_n ./ log(n);
    leyenda = ['$T(n) / log(n)$'];
else
    error('Parámetro "divisor" inválido.');
    exit(-1);
end

figure;
hold on;

if(s.ticks_x)
    xticks = [1:ceil(length(label_n)/4):length(label_n) length(label_n)];
    set(gca, 'xtick', xticks);
    set(gca, 'xticklabel', label_n'(xticks));
end

if(strcmp(s.escala, 'lineal'))
    plot(n, expr);
elseif(strcmp(s.escala, 'logaritmica'))
    semilogy(expr);
else
    error('Parámetro "escala" inválido.');
    exit(-1);
end

if(strcmp(s.ajuste, 'none'))
    % No hacer nada.
elseif(strcmp(s.ajuste, 'n'))
    expr_ajuste   = s.coef * n;
    leyenda = [leyenda; '$O(n)$'];
elseif(strcmp(s.ajuste, 'log(n)'))
    expr_ajuste    = s.coef * log(n);
    leyenda = [leyenda; '$O(log(n))$'];
elseif(strcmp(s.ajuste, 'n*log(n)'))
    expr_ajuste    = s.coef * n .* log(n);
    leyenda = [leyenda; '$O(n * log(n))$'];
else
    error('Parámetro "ajuste" inválido.');
    exit(-1);
end

if(exist('expr_ajuste'))
    plot(n, expr_ajuste, 'color', 'red', 'linestyle', ':');
end

legend(leyenda);
legend('boxon');
legend('location', 'northwest');

title(s.titulo);

xlabel(s.xlabel);

if(strcmp(s.unidad, 'milisegundos'))
    ylabel('Tiempo [$mS$]');
else
    ylabel('Tiempo [$\mu S$]');
end

xlim([s.inicio rows(t_n)]);

if(!strcmp(s.ylim, 'none'))
    ylim([0 s.ylim]);
end

hold off;