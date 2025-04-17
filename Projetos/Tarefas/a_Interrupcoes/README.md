# Tarefa Interrupções
---
## Descrição

Programa em linguagem C que implementa um contador decrescente controlado por interrupções. Utiliza dois botões (GPIO5 e GPIO6) e um display OLED.

---

## Funcionamento

    O sistema incia na tela de "Boas Vindas" e aguarda para iniciar

    Botão A (GPIO5): Inicia a contagem de 9 até 0, com decremento de 1 por segundo.

    Botão B (GPIO6): Durante a contagem, registra os cliques. O total é exibido no display.

    Ao chegar em 0, o sistema congela e mostra:

        Tempo 0

        Clicks (Total de cliques registrados)

    Um novo clique no Botão A reinicia todo o processo.

    Caso o sistema fique sem interação por mais de 10s ele retorna para o "Boas Vindas"

---