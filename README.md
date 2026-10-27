# 🎓 Sistema Acadêmico em C

Sistema de gerenciamento de alunos e notas executado no terminal, desenvolvido em **C** para a disciplina de **Programação Estruturada**. Os dados são salvos em arquivo binário e continuam disponíveis na próxima execução.

## ✨ Funcionalidades

- **Cadastrar** a turma (nome, código único e notas)
- **Buscar** aluno pelo código
- **Atualizar** dados, adicionar novos alunos ou alterar o status (Ativo, Trancado, Transferido, Formado)
- **Remover** aluno
- **Listar** todos os alunos ou gerar o **relatório da turma**

Cada aluno tem 3 avaliações (Ac1, Ac2, Af) em 5 disciplinas: Português, Matemática, Ciência, História e Geografia.

## 🧠 Conceitos aplicados

Structs aninhadas, ponteiros, alocação dinâmica (`malloc`, `realloc`, `free`), funções, arquivos binários (`fread`, `fwrite`, `fseek`) e validação de entrada.

## ⚙️ Como compilar e executar

Requer um compilador C (como GCC) e **Windows** (usa `cls` e `pause`).

```bash
gcc main.c -o sistema_academico
sistema_academico.exe
```

> No Linux/macOS, troque `system("cls")` por `system("clear")` e `system("pause")` por `getchar()`.

## 📊 Relatório da turma

Mostra a média de cada disciplina `(Ac1 + Ac2 + Af) / 3`, a média geral e a situação de cada aluno (aprovado com média ≥ 6,0 em todas). No final, traz a média da turma, o total de aprovados e reprovados, a quantidade por status e o aluno com a maior média.

## 💾 Dados e validações

- Os dados ficam em `alunos.dat` e são carregados ao iniciar.
- Se o arquivo estiver inválido, ele é renomeado para `alunos_corrompido.dat` para não perder dados.
- Entradas inválidas são recusadas: letras no lugar de números, notas fora de 0 a 10 e códigos repetidos.

## 👨‍💻 Autor

**Danilo Yoshihiro Marcelo Goya**, Engenharia da Computação, FACENS
GitHub: [danilogoya11-spec](https://github.com/danilogoya11-spec)
