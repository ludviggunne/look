# Look
A simple search engine for local directories, using [tf-idf](https://en.wikipedia.org/wiki/Tf%E2%80%93idf).
All files are treated like plain text, and all queries are single words (for now).

# Usage
Start by creating an index for the root directory:
```bash
look index <root>
```
Start the server:
```bash
look
```
The server reads queries from standard input and writes a list of matching files, in
order of relevance, to standard output. The list is prefixed by the number of matched files.
