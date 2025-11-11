#!/bin/bash

# Script to check GitHub Actions CI/CD status
# Note: This requires 'gh' CLI tool to be installed and authenticated

set -e

echo "🔍 GitHub Actions CI/CD Status Check"
echo "=================================="

# Check if gh CLI is available
if ! command -v gh &> /dev/null; then
    echo "❌ 'gh' CLI not found. Please install GitHub CLI:"
    echo "   brew install gh   # macOS"
    echo "   sudo apt install gh # Ubuntu"
    echo ""
    echo "Then authenticate with: gh auth login"
    exit 1
fi

# Check if authenticated
if ! gh auth status &> /dev/null; then
    echo "❌ Not authenticated with GitHub. Run: gh auth login"
    exit 1
fi

echo "✅ GitHub CLI available and authenticated"
echo ""

# Get latest workflow runs
echo "📋 Latest Workflow Runs:"
echo "======================"

# Get workflow runs for the main branch
gh run list --repo aokihu/claude-code-env-switcher --limit 5 --branch main

echo ""
echo "🏷️  Latest Tagged Runs:"
echo "====================="

# Get workflow runs for tags
gh run list --repo aokihu/claude-code-env-switcher --limit 3 --branch v2.0.2

echo ""
echo "📦 Latest Releases:"
echo "=================="

# List latest releases
gh release list --repo aokihu/claude-code-env-switcher --limit 3

echo ""
echo "💡 To view detailed workflow status:"
echo "   https://github.com/aokihu/claude-code-env-switcher/actions"
echo ""
echo "💡 To view releases:"
echo "   https://github.com/aokihu/claude-code-env-switcher/releases"