# Flux, Tăieturi Minime și Cuplaje: Aplicații în Optimizare și Măsurarea Performanței

**Absolvent**: *Bucă Mihnea-Vicențiu*

**Coordonator științific**: *Lect. dr. Dumitran Adrian Marius*

În acest repository analizăm algoritmi clasici și moderni pentru rezolvarea problemelor de flux maxim, tăieturi minime și cuplaje în grafuri bipartite. Studiul include implementări și comparații între metode fundamentale precum [Ford-Fulkerson](https://en.wikipedia.org/wiki/Ford–Fulkerson_algorithm), [Edmonds-Karp](https://en.wikipedia.org/wiki/Edmonds–Karp_algorithm), [Push-Relabel](https://en.wikipedia.org/wiki/Push–relabel_maximum_flow_algorithm), [Dinic](https://en.wikipedia.org/wiki/Dinic's_algorithm), etc. Pentru evaluarea performanței, am generat rețele cu diverse caracteristici structurale (de la instanțe *sparse* (rare) la *dense*), precum și teste standard preluate din colecții publice recunoscute din cultura informatică precum [CSES](https://cses.fi/problemset/), [LibreOJ](https://loj.ac/), [Infoarena](https://infoarena.ro/arhiva-educationala) și alte arhive accesibile, care oferă cazuri de test curate și relevante pentru a putea fi folosite în validarea algoritmilor utilizați.

Toate implementările sunt realizate în limbajul de programare `C++`, folosind exclusiv biblioteci standard, pentru a facilita o înțelegere detaliată a mecanismelor algoritmice și a optimizărilor aplicate. Am analizat avantajele și limitările fiecărei abordări și am propus îmbunătățiri punctuale, precum structuri de date native, euristici pentru selecția muchiilor și ajustări specifice pentru creșterea eficienței și scalabilității.

Structura lucrării urmărește o organizare clară pe clase de algoritmi, accentuând contextul aplicabilității acestora în probleme de optimizare și alocare de resurse. Analiza este orientată atât către înțelegerea detaliată a mecanismelor interne ale fiecărui algoritm, cât și către compararea comportamentului lor în scenarii variate.

La finalul acestui demers, scopul este identificarea unei soluții algoritmice generaliste, cât mai robuste și eficace într-un spectru larg de instanțe, un algoritm *multipurpose*, care oferă un echilibru optim între performanță și scalabilitate, indiferent de caracteristicile rețelei analizate.