function renderScratchBlocks() {
  if (typeof scratchblocks === 'undefined') return;

  scratchblocks.renderMatching('.blocks', {
    style: 'scratch3'
  });

  // SVG Injection for Custom Pybricks Icons
  const cubeSVG = `data:image/svg+xml;utf8,<svg width="24" height="24" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg"><path d="M12 12 L12 22 M12 12 L3 7 M12 12 L21 7 M3 7 L3 17 L12 22 L21 17 L21 7 L12 2 Z" stroke="white" stroke-width="2" stroke-linejoin="round" stroke-linecap="round" fill="none"/></svg>`;
  
  const importSVG = `data:image/svg+xml;utf8,<svg width="24" height="24" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg"><polygon points="12,14 7,8 10,8 10,2 14,2 14,8 17,8" fill="white"/><rect x="3" y="17" width="18" height="5" rx="1" fill="white" /><rect x="6" y="15" width="4" height="2" fill="white" /><rect x="14" y="15" width="4" height="2" fill="white" /></svg>`;

  const hubSVG = `data:image/svg+xml;utf8,<svg width="24" height="24" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg"><rect x="3" y="9" width="18" height="11" rx="2" stroke="white" stroke-width="2" fill="none"/><rect x="5" y="6" width="4" height="3" stroke="white" stroke-width="2" fill="none"/><rect x="15" y="6" width="4" height="3" stroke="white" stroke-width="2" fill="none"/><path d="M 8 15 L 16 15" stroke="white" stroke-width="2" stroke-dasharray="2 2"/></svg>`;
  
  const playSVG = `data:image/svg+xml;utf8,<svg width="24" height="24" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg"><path d="M 8 6 L 8 18 L 18 12 Z" fill="white"/></svg>`;

  // Target all SVG paths and use elements that might be the icons
  const iconElements = document.querySelectorAll('.blocks svg path, .blocks svg use, .blocks svg image');
  iconElements.forEach(el => {
    const fill = (el.getAttribute('fill') || '').toUpperCase();
    const href = (el.getAttribute('href') || el.getAttributeNS('http://www.w3.org/1999/xlink', 'href') || '').toLowerCase();
    const className = (el.getAttribute('class') || '').toLowerCase();
    
    const isGreenFlag = fill === '#4CBF56' || className.includes('green') || href.includes('green');
    const isStopSign = fill === '#EC5959' || className.includes('stop') || href.includes('stop');
    const isHub = href.includes('turnright');
    const isPlay = href.includes('turnleft');
    
    if (isGreenFlag || isStopSign || isHub || isPlay) {
      // Create an <image> element
      const img = document.createElementNS('http://www.w3.org/2000/svg', 'image');
      
      let iconSrc = cubeSVG;
      if (isStopSign) iconSrc = importSVG;
      if (isHub) iconSrc = hubSVG;
      if (isPlay) iconSrc = playSVG;
      
      img.setAttribute('href', iconSrc);
      
      const transform = el.getAttribute('transform');
      if (transform) {
        img.setAttribute('transform', transform);
      }
      
      img.setAttribute('x', '0');
      img.setAttribute('y', '0'); 
      img.setAttribute('width', '24');
      img.setAttribute('height', '24');
      
      if (el.parentNode) {
        el.parentNode.replaceChild(img, el);
      }
    }
  });
}

window.$docsify = window.$docsify || {};
window.$docsify.plugins = window.$docsify.plugins || [];
window.$docsify.plugins.push(function(hook, vm) {
  hook.doneEach(function() {
    // We must wait a bit for docsify to actually insert the DOM nodes
    setTimeout(renderScratchBlocks, 200);
  });
});
