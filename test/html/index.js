import Sharpen from 'sharpen/core';

// fix loading path;
new Sharpen({
    locateFile: f => ('/dist/' + f)
}, instance => {
    document.getElementById('btn').addEventListener('click', () => {
        // do DOM patch render with "Sharpen";
        instance.patch(
            document.getElementById('native'), 
            document.getElementById('patch')
        );
        window.Sharpen = instance;
    });
});
