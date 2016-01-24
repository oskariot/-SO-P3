# Pracownia P3 - Systemy Operacyjne

## Problem przeprawy przez rzekę (*River crossing*)
Pewna przeprawa przez rzekę dzielona jest zarówno przez linuxowych Hakerów, jak i pracowników Microsoftu. Do przeprawy używana jest **jedna** łódka, która mieści tylko **czterech** programistów i zawsze płynie **w pełni** załadowana. Łódka napędzana jest przez **jedną** osobę wiosłującą.<br />
Aby zagwarantować bezpieczeństwo programistów w łódce, nie może dojść do następujących sytuacji:

1. Trzech linuxowych Hakerów przebywa w łódce z trzema pracownikami Microsoftu.
2. Trzech pracowników Microsoftu przebywa w łódce z trzema linuxowymi Hakerami.

Wszystkie pozostałe kombinacje są bezpieczne.<br />
Dodatkowo programiści wprowadzili zasadę "kto pierwszy ten lepszy", a więc ostatnia zgłoszona do łódki osoba będzie wioślarzem. Łódka ma być gospodarowana **optymalnie**, tzn. powinnna odpłynąć, gdy zgłoszone osoby będą mogły utworzyć poprawny ładunek. Ponieważ może się okazać, że wśród ostatnich programistów nie da się utworzyć ładunku (np. pozostanie trzech programistów), powinni oni zaniechać prób dostania się na łódź.
#### Zadanie
Zaimplementować rozwiązanie powyższego konfliktu zasobów z użyciem wątków. Każdy programista powinien być reprezentowany przez osobny wątek Hakera / pracownika Microsoftu wykonującym odpowiednio jedną z funkcji *linHackerArrives()* i *windowserArrives()*. Wątek może zakończyć działanie, gdy wystąpi jedna z sytuacji:

1. Programista przeprawi się przez rzekę.
2. Programista nie ma możliwości przeprawy przez rzekę (pula programistów skończyła się i nie może on dobrać sobie odpowiedniej ekipy).


