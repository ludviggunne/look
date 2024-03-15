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
Or run a single query:
```bash
look -q <query>
```
Output is one line with the number of matches, followed by the matching files.
