# Contribution Guide <!-- omit in toc -->
Thanks for taking the time to look into our project. There is plenty of ways to contribute. 

## I want to report a bug.
We use GitHub for bug-tracking purposes. Please open a new [Issue on GitHub](https://github.com/sdavtaker/ia-devs-cd-poc/issues).
Please include as much detail as possible, including but not limited to:
- The toolchain and platform used
- Code samples to reproduce the bug
- Steps to reproduce the bug

## I want to contribute patches to bugs or code improvements.
If you have already implemented the code to contribute, please open a [Pull Request on GitHub](https://github.com/sdavtaker/cadmium/pulls) with the change.

On the other hand, if you are yet to write the code you want to contribute and would like to have some guidance from us. Open a new [Issue on GitHub](https://github.com/sdavtaker/ia-devs-cd-poc/issues) describing what change you wish to contribute, and we will provide advice.

In addition to reviewing functionality, we will validate code style and test coverage for every Pull Request we review. The code style the project uses is LLVM as enforced by clang-format, and it is evaluated automatically by GitHub actions in every Pull Request.

If you want to contribute but do not know where to start, we keep a list of good-to-start open issues with the label [good first issue](https://github.com/sdavtaker/ia-devs-cd-poc/labels/good%20first%20issue).

## I want to contribute documentation, GitHub issue templates, wiki content, and other project-related artifacts outside building code. 
The [doc](./doc) directory of the project contains all project documentation that is part of releases. Submit changes by cutting a new [Pull Request on GitHub](https://github.com/sdavtaker/cadmium/pulls). 

For artifacts not expected to be included in the releases, like GitHub Issues templates, Pull Request templates, and Wiki content, open a new [Issue on GitHub](https://github.com/sdavtaker/ia-devs-cd-poc/issues).

## I want to propose changes to the project design or roadmap.
We highly welcome proposals for changes to minor and significant parts of the project the same. To keep our decisions consistent across the contributor community, we have documents for [design goals](./design/goals.md), [tenets](./design/tenets.md), [high-level architecture](./design/architecture.md) and [FAQs](./design/FAQs.md) in the [design](./design) directory. We have feature-specific docs in some cases in the same directory. We encourage reading those docs before submitting a design or roadmap change proposal to have the fastest and best experience on getting this kind of changes merged.

For submitting, feel free to cut a new [Pull Request on GitHub](https://github.com/sdavtaker/cadmium/pulls) with the proposed changes, or if you want to discuss ideas first, create a new [Issue on GitHub](https://github.com/sdavtaker/ia-devs-cd-poc/issues).

## I want to add a question to the project FAQ.
Our preferred option is to open a new [Pull Request on GitHub](https://github.com/sdavtaker/cadmium/pulls) to add the question to our FAQ. And if you know the answer, feel free to include it in the Pull Request. Another option is to cut a new [Issue on GitHub](https://github.com/sdavtaker/ia-devs-cd-poc/issues) to discuss it and document the conclusion for FAQ inclusion.
## I want to set up my development environment to contribute to the project.
Follow the build from source instructions in [README.md](./README.md). To run tests, run `ctest`.
