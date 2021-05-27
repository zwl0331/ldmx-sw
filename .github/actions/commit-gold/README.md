# commit-gold Action

This is meant to be run after a `validate` action. 
We assume that the histogram files generated by the `validate` action
are each in their own "sample" directory where the `gold.root`
golden histogram file should be committed.

In order to [push a commit with a GitHub action](https://github.com/actions/checkout#Push-a-commit-using-the-built-in-token),
we need to checkout the entire repo.

## In-Workflow Example

This is meant to be paired with the validate action, 
so we generate the golden histograms in the exact same way as we will when validating them.

```yaml
    - name: Run Validation
      uses: ./.github/actions/validate
      with:
        sample: inclusive

    # other sample runs

    - name: Commit Gold
      uses: ./.github/actions/commit-gold
```