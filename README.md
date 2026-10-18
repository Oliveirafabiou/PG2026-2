# Processamento Gráfico 2026-2 — Atividades Práticas C++ / OpenGL

Este repositório contém a implementação das atividades e exercícios da disciplina de **Processamento Gráfico**, organizados em um projeto C++ modularizado com **OpenGL (GLFW / GLEW / GLM)** no Visual Studio.

---

## 📁 Estrutura do Projeto

O projeto utiliza uma arquitetura limpa que separa o núcleo de abstração gráfica (`core/`) dos módulos específicos de cada lista de exercícios:

```text
PG2026-2/
├── Project1/
│   ├── core/                  # Módulo central de abstração OpenGL
│   │   ├── window.h / window.cpp    # Gerenciamento de janela GLFW e inicialização do GLEW
│   │   ├── shader.h / shader.cpp    # Compilação, vinculação e manipulação de Uniforms GLSL
│   │   └── mesh.h / mesh.cpp        # Encapsulamento de VAO/VBO e chamadas de renderização
│   ├── Lista1/                # Módulo da Lista 1
│   │   ├── Lista1.h
│   │   ├── Lista1Ex1.cpp      # Primitivas gráficas e modos de rasterização
│   │   └── Lista1Ex2.cpp      # Geometrias paramétricas interativas
│   ├── Lista2/                # Módulo da Lista 2
│   │   ├── Lista2.h
│   │   └── Lista2.cpp         # Gerenciamento de Viewports, quadrantes e submenus
│   ├── Lista3/                # Módulo da Lista 3
│   │   ├── Lista3.h
│   │   └── Lista3.cpp         # Construção interativa de triângulos e física/animação
│   ├── main.cpp               # Menu principal de navegação via console
│   ├── Project1.vcxproj       # Configuração do projeto do Visual Studio
│   └── Project1.vcxproj.filters
├── CMakeLists.txt             # Arquivo de build para suporte a CMake
└── README.md
```

---

## 📖 Instruções de Uso e Teste dos Exercícios

Ao executar a aplicação, um **Menu Principal** interativo será exibido no console para seleção da lista desejada.

### 🔹 Lista 1 — Exercício 1 (`Lista1Ex1.cpp`)
* **Descrição:** Renderização de primitivas utilizando diferentes modos de preenchimento sobre a mesma malha.
* **Como Testar:**
  1. Digite `1` no menu principal para abrir o submenu da Lista 1 e escolha o Exercício 1.
  2. Uma janela OpenGL será aberta exibindo as primitivas desenhadas em lote usando modos como preenchimento (`GL_FILL`), contorno (`GL_LINE`) e pontos (`GL_POINTS`).
  3. Pressione a tecla **`ESC`** para fechar a janela gráfica e retornar ao menu.

---

### 🔹 Lista 1 — Exercício 2 (`Lista1Ex2.cpp`)
* **Descrição:** Gerador de geometrias paramétricas interativas (Círculo, Octógono, Pentágono, Pac-Man, Fatia de Pizza, Estrela e Espiral).
* **Como Testar e Interagir:**
  1. No submenu da Lista 1, selecione o Exercício 2.
  2. A janela gráfica abrirá exibindo a primeira forma geométrica (Círculo).
  3. **Interação por Teclado:** Pressione as teclas de **`SETA PARA A DIREITA`** ou **`SETA PARA A ESQUERDA`** no teclado para alternar ciclicamente entre as **7 geometrias paramétricas**.
  4. O terminal exibirá o nome e o número da forma carregada a cada troca.
  5. Pressione a tecla **`ESC`** para fechar a janela e retornar ao menu.

---

### 🔹 Lista 2 — Múltiplas Visões e Viewports (`Lista2.cpp`)
* **Descrição:** Mapeamento de coordenadas NDC para recortes específicos da janela utilizando `glViewport`.
* **Como Testar e Interagir:**
  1. No menu principal, escolha a opção da Lista 2.
  2. Um submenu exclusivo da Lista 2 será apresentado no console:
     * **Opção 1 (Quadrante Único):** Permite escolher em qual dos 4 quadrantes da tela (Q1 - Superior Direito, Q2 - Superior Esquerdo, Q3 - Inferior Esquerdo, Q4 - Inferior Direito) o objeto será renderizado. Inclui a opção **`5. Voltar ao Menu Anterior`** para retornar sem abrir a janela.
     * **Opção 2 (Todos os Quadrantes):** Renderiza o objeto simultaneamente nos 4 quadrantes da tela, utilizando cores independentes para cada Viewport.
     * **Opção 3 (Voltar):** Retorna ao Menu Principal.
  3. Pressione a tecla **`ESC`** para fechar qualquer janela gráfica aberta e voltar ao submenu da Lista 2.

---

### 🔹 Lista 3 — Interação via Mouse e Animações (`Lista3.cpp`)
* **Descrição:** Construção interativa de triângulos através de cliques do mouse com animação contínua de translação e rotação individual.
* **Como Testar e Interagir:**
  1. No menu principal, escolha a opção da Lista 3.
  2. **Criação de Vértices:** Clique com o **`Botão Esquerdo do Mouse`** na área da janela para marcar os pontos do triângulo.
     * Nos **2 primeiros cliques**, os pontos serão marcados na tela com um destaque visual na cor sorteada para aquele triângulo.
     * Ao realizar o **3º clique**, o triângulo é finalizado e passa a existir como um objeto dinâmico.
  3. **Animação em Tempo Real:** Assim que o triângulo é completado, ele inicia automaticamente:
     * **Translação:** Movimento contínuo e suave pela tela, rebatendo nas bordas da janela.
     * **Rotação:** Giro constante em torno do seu próprio centroide.
  4. Múltiplos triângulos podem ser criados em sequência na mesma execução, cada um com sua própria cor, velocidade e ângulo de movimento.
  5. Pressione a tecla **`ESC`** para encerrar a simulação e retornar ao menu principal.

---

## ⌨️ Tabela Resumo de Atalhos e Comandos

| Local | Ação | Atalho / Comando |
| :--- | :--- | :--- |
| **Global** | Fechar janela gráfica atual e voltar | `ESC` |
| **Lista 1 Ex 2** | Alternar para a próxima geometria | `Seta para Direita` |
| **Lista 1 Ex 2** | Alternar para a geometria anterior | `Seta para Esquerda` |
| **Lista 2** | Navegar pelos submenus e quadrantes | Digitar opção no console (`1` a `5`) |
| **Lista 3** | Marcar vértice / Criar triângulo | `Clique com Botão Esquerdo do Mouse` |

---

## 🛠️ Requisitos e Configuração do Ambiente

* **Compilador C++:** Suporte a C++17 ou superior.
* **IDE Recomendada:** Visual Studio 2022 (com o pacote *Desenvolvimento para Desktop com C++*).
* **Dependências de Terceiros**:
  * **GLFW 3** (gerenciamento de janelas e eventos)
  * **GLEW** (carregamento de extensões OpenGL)
  * **GLM** (computação gráfica e matemática matricial)

---

## 🚀 Como Compilar e Executar

1. Abra o arquivo de solução/projeto (`Project1.vcxproj`) no **Visual Studio**.
2. Certifique-se de selecionar a plataforma **x64** e a configuração **Debug** ou **Release**.
3. Pressione `F5` (ou `Ctrl + F5`) para compilar e iniciar a aplicação.
4. Utilize a janela de console para navegar interativamente pelas listas e exercícios.

---

## 👤 Autor

* **Fábio O. Silveira**
* Disciplina: Processamento Gráfico (2026-2)
