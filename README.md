# Pracownia P3 - Systemy Operacyjne

## Problem przeprawy przez rzekę (*River crossing*)
Pewna przeprawa przez rzekę dzielona jest zarówno przez linuxowych Hakerów, jak i pracowników Microsoftu. Do przeprawy używana jest **jedna** łódka, która mieści tylko **czterech** programistów i zawsze płynie **w pełni** załadowana. Łódka napędzana jest przez jedną osobę wiosłującą.<br />
Aby zagwarantować bezpieczeństwo programistów w łódce, nie może dojść do następujących sytuacji:

1. Trzech linuxowych hakerów przebywa w łódce z trzema pracownikami Microsoftu.
2. Trzech pracowników Microsoftu przebywa w łódce z trzema linuxowymi Hakerami.

Wszystkie pozostałe kombinacje są bezpieczne.<br />
Dodatkowo programiści wprowadzili zasadę "kto pierwszy ten lepszy", a więc ostatnia zgłoszona do łódki osoba będzie wioślarzem. 

Two procedures are needed: HackerArrives and EmployeeArrives, called by a hacker
