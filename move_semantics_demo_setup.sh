# Initialize (skip if already a git repo)
git init

# Create a .gitignore (see content below)
# then stage everything
git add .

# Commit
git commit -m "Add educational move semantics library and demos"

# Create a new empty GitHub repo (via the website).
# Add it as remote:
git remote add origin https://github.com/rebcabin/educational.git
# or with SSH:
# git remote add origin git@github.com:<your-username>/<your-repo>.git

# Push and set upstream (replace main with your branch name if different)
git branch -M main
git push -u origin main

# Optional: create a tag for this checkpoint and push it
git tag v0.1-move-semantics-demo
git push origin v0.1-move-semantics-demo
